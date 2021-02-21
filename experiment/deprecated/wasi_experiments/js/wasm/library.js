//**************************************************************************************
// memory

/*
Linear memory, shared for JS and C side
The first page is reserved for allocation done from JS side.
First byte of the second is for C allocation(manage by the implementation heap in C side)
*/

class Memory
{
 constructor(nbPages/*, heap_size_pages_max*/)
 {
   // See https://github.com/WebAssembly/design/blob/master/Rationale.md#linear-memory-resizing
   this.PAGE_SIZE = 64 * 1024;

   // Equivalent of the break program location:
   // First byte after the available data
   this.m_JS_data_segment = this.PAGE_SIZE;
   this.m_C_data_segment = this.m_JS_data_segment + (nbPages * this.PAGE_SIZE);

   this.m_JS_allocated_bytes = 1; // Reserve NULL adress
   this.m_C_allocated_bytes = 0;

   this.memory = new WebAssembly.Memory({
       initial: nbPages+1, /* First page is reserved for the JS side */
       maximum: nbPages+1 // TODO: Manage dynamic page
   });
 }

 // Should be use only from C call
 brk(end_data_segment)
 {
    var msg = `Memory::brk(${end_data_segment})`;
    if (end_data_segment > this.m_C_data_segment)
    {
      var diff = end_data_segment - this.m_C_data_segment;
      var pages_needed = Math.ceil(inc / this.PAGE_SIZE);
      this.m_memory.grow(pages_needed+1 /* First page is reserved for the JS side */);
      memory = this.m_memory;
    }
    if (end_data_segment != 0)
      this.m_C_data_segment = end_data_segment;
    console.log(msg + " => 0");
    return 0;
 }

//Simple mmap...
//Should only be call from C
 mmap(length)
 {
    var msg = `Memory::mmap(${length})`;
    // First page is reserved for JS allocation
    if ((this.m_JS_data_segment + this.m_C_allocated_bytes + length) > this.m_C_data_segment)
    {
      //TODO: this.m_memory.grow
      return -1;
    }
    this.m_C_allocated_bytes += length;
    console.log(msg + " => " + this.m_JS_data_segment);
    return this.m_JS_data_segment;
 }

 // JS Memory "stack" filled by JS and consumed by C
 //Should only be call from JS
 // TODO: Each call is a leak
 malloc4JS(size)
 {
    var msg = `Memory::malloc4JS(${size})`;
    if (size == 0)
      throw new RangeError("JS Memory.malloc4JS: Invalid size '0'");
    if ((this.m_JS_allocated_bytes + size) > this.m_JS_data_segment)
      throw new Error("JS Memory.malloc4JS: No memory left...");
    this.m_JS_allocated_bytes += size;
    var ptr = this.m_JS_allocated_bytes - size;
    console.log(msg + " => " + ptr);
    return ptr;
 }
 // Heap access helpers.
  get memory()
  {
    return this.m_memory;
  }
  set memory(memory)
  {
    this.m_memory = memory
    this.m_heap = this.m_memory.buffer;
    this.m_heap_uint8 = new Uint8Array(this.m_heap);
    this.m_heap_uint32 = new Uint32Array(this.m_heap);
    this.m_C_data_segment = this.m_heap.byteLength;
  }

  get heap_uint8()
  {
    return this.m_heap_uint8;
  }
  get heap_uint32()
  {
    return this.m_heap_uint32;
  }

  getCharAt(ptr)
  {
    return String.fromCharCode(this.m_heap_uint8[ptr]);
  }

  getStringAt(ptr, len = -1)
  {
    var str = "";
    // Java data segment is always less than the C data segment
    var end = this.m_C_data_segment;
    if (len != -1)
        end = ptr + len;
    for (var i = ptr; i < end && this.m_heap_uint8[i] != 0; ++i)
      str += this.getCharAt(i);
    return str;
 }
}

if(typeof module !== 'undefined' && module.exports)
	module.exports = Memory;


//**************************************************************************************
// download
try
{
	// For Nodejs
	fs = require('fs');
}
catch(ex)
{
	fs = undefined;
}

class Download
{
	static get WASMJSON_EXT() {return "wasm.json";}
	static get WASMJS_EXT() {return "wasm.js";}
	static get WASMSO_EXT() {return "so";}
	static get ROOTDIR() {if(fs!=undefined)return "sap_bobj/enterprise_xi40";else return "";}
	static get PLATFORM() {return "wasm_clang";}
	static get BINDIR() {return this.ROOTDIR+"/"+this.PLATFORM;}

	//TODO: use cache
	static importFile( url, binary )
	{
		var data;
		if(fs != undefined)
		{
			try
			{
				if(binary == true)
				{
					data = fs.readFileSync(url);
				}
				else
				{
					data = fs.readFileSync(url, "utf8");
				}
			}
			catch(ex)
			{
				console.error(ex.message);
			}
		}
		else
		{
			try
			{
				var req = new XMLHttpRequest();
				req.open( 'GET', url, false ); // <-- the 'false' makes it synchronous
				if(binary==true)
					req.overrideMimeType('text\/plain; charset=x-user-defined');
				req.send(null);
				if(req.status != 200)
				{
					return null;
				}
				data = req.response;
				if(binary)
					return this._stringToArrayBuffer(data);
			}
			catch(ex)
			{
				console.error("importFile("+url+"): " + ex.message + ex);
				return null;
			}
		}
		return data;
	}

	static importWasmMetadata( libname )
	{
		let url = libname+"."+this.WASMJSON_EXT;
		let metadata = this.importFile(url)
		if(metadata == null)
			throw new ReferenceError(`No metadata file find: '${url}'`)
		return JSON.parse(metadata);
	}

	static _stringToArrayBuffer(str)
	{
		var buf = new ArrayBuffer(str.length);
		var bufView = new Uint8Array(buf);

		for (var i=0, strLen=str.length; i<strLen; i++)
		{
			bufView[i] = str.charCodeAt(i);
		}

		return buf;
	}
}

if(typeof module !== 'undefined' && module.exports)
	module.exports = Download;



//**************************************************************************************
// library_base
//Use by libc.wasm.js to identify the caller library
//   inityalize in this class
//   should be set before all call to the JS call in libc.wasm.js
//TODO: find a better way than a global var...
callerLibrary = null;

//Depend on :
//		/js/helpers/download.js
//				importWasmMetadata
//		/js/wasm/memory.js
//				Memory


if (typeof Download.importWasmMetadata != 'function')
	throw new ReferenceError("missing js/helpers/download.js");
if (typeof Memory != 'function')
	throw ReferenceError("missing js/wasm/memory.js");

class LibraryBase
{
	constructor(arg0, arg1, arg2)
	{
		if(arg2===undefined)
		{
			if(arg0.indexOf(Download.ROOTDIR) != -1)
				this._path=arg0;
			else
				this._path = Download.ROOTDIR+arg0;
			this._memory = arg1;
			this._folder = this._path.substring(0, this._path.lastIndexOf("/"));
		}
		else
		{
			this._folder = arg0;
			this._path = arg0+"/"+arg1;
			this._memory = arg2;
		}
		this._name = this._path.substring(this._path.lastIndexOf("/")+1);
		this._importsFunctions = {env:{}};
		this._importsSymbols = {env:{}};
		
		this._exportsFunctions = {};  //fctname:{fct:therealfunction};
		this._exportsIndirectFunctionsTable = null;

		this._exportsTables = {};
		this._exportsSymbols = {};
		this._exportsSymbols_wasm_js = null;

		this._wasm_js = null;
		this._wasm_so = null;
		this._version = null;
		this._dependencies = {};
	}

	//TODO: this parameter imports should be part of the Dyn Laoder
	//TODO: See https://linux.die.net/man/3/dlopen parameter need to implement flags behavior/
	//     For the moment the default behavior is RTLD_GLOBAL | RTLD_NOW
	loadModule(imports)
	{
		try
		{
			console.log("********************************************************************************************************");
			console.log("Start to load Library :" + this._path);
			// Load metadata first
			this._loadMetadata();
			// Load dependencies coming from metadata defintion
			// Done first because this current code contains the Dyn Loader (TODO: need to be separate)
			console.log("******************************************");
			console.log("Start to load dependencies");
			for ( var dep in this._dependencies)
			{
				let libPath = this._dependencies[dep];
				console.log(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
				console.log(libPath);
				let library = new Library(libPath);
				library.loadModule(imports);
				console.log("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
			}
			console.log("Finish to load dependencies");

			// Load code of this module
			if(this._wasm_js)
				this._loadWasmJS(imports);
			if(this._wasm_so)
				this._loadWasmSO(imports);
			
			
			console.log("******************************************");
			console.log("Add symbols to table");
			//this.linkLibrary(this);

			console.log("******************************************");
			console.log("Add symbols of dependencies to table");
			for ( var dep in this._dependencies)
			{
				let libName = this._dependencies[dep];
				console.log(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
				console.log(libName);
				this.linkLibrary(dep);
				console.log("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
			}
		}
		catch (e)
		{
			console.error(e);
			throw e;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// Private methods
	_loadMetadata()
	{
		console.log("******************************************");
		console.log("Loading metadata for :" + this._path);
		let metadata = Download.importWasmMetadata(this._path);
		if(!metadata.version)
			throw new ReferenceError(`Missing version field from metadata of '${this._path}'`)
		this._version = metadata.version;
		if(metadata.wasm_js)
			this._wasm_js = Download.ROOTDIR+metadata.wasm_js;
		if(metadata.wasm_so)
			this._wasm_so = Download.ROOTDIR+metadata.wasm_so;
		if(metadata.libs)
			this._dependencies = metadata.libs;
		console.log(metadata);
	}

	// return boolean
	validate(arrayBuffer)
	{
		return WebAssembly.validate(arrayBuffer);
	}

	extractSymbolsAndImplFctsJS(imports, jsTxt)
	{
		console.log("********************************************************************************************************");
		console.log("export from module (JS side):" + this._path);

		var jssymbols = {};
		var getJSExports = undefined;
		//TODO: eval the JS only once by js... or find a better way than eval.apply
		try
		{
			if(typeof module !== 'undefined' && module.exports)
				getJSExports = require("../../../../"+this._wasm_js);
			else
			{
				var b = eval.call(this, jsTxt );
				getJSExports = window.getJSExports;
			}
		}
		catch (e)
		{
			throw RangeError(`${this._name} Error during evauation of ${this._wasm_js}`, e);
		}
		if (typeof getJSExports == 'undefined')
			throw ReferenceError(`${this._name} Missing defintion of the function getJSExports(exports) in ${this._wasm_js}`)
		var exports = {};
		getJSExports(exports);
		var jssymbols = exports[this._name];
		if(!jssymbols)
			throw ReferenceError(`${this._name} Missing initialisation of exports.${this._name} in ${this._wasm_js}:fct(${getJSExports})`)

		for (var group in jssymbols)
		{
			console.log(group, jssymbols[group]);
		}

		// TODO: this should be manage by the Dyn Linker
		// Resolve import module with wasm.js exportsSymbols
		for(var group in jssymbols)
		{
			for(var f in jssymbols[group])
				imports.env[f] = jssymbols[group][f];
		}
		this._exportsSymbols_wasm_js = jssymbols;
	}

	extractNatifSymbols(module, imports)
	{
		console.log("******************************************");
		console.log("customSectionsName from compiled module (Native side):" + this._path);
		let customSectionsName = WebAssembly.Module.customSections(module, "name");
		for (var f in customSectionsName)
		{
			console.log(customSectionsName[f]);
		}
		
		// TODO: Need to verify specifications :
		//	export fct list match index of function(eq fct ptr)) in term of order and base index
		//  index is based starting after the last imported function... :?
		var indexFct = 1;  // Start to 1, 0 is null ptr
		
		console.log("******************************************");
		console.log("import from compiled module (Native side):" + this._path);
		let importsSymbols = WebAssembly.Module.imports(module);
		for (var f in importsSymbols)
		{
			console.log(importsSymbols[f]);
			if( importsSymbols[f].kind == "function")
			{
				this._importsFunctions["env"][f] = importsSymbols[f];
				indexFct++;
			}
			else
				this._importsSymbols["env"][f] = importsSymbols[f];
		}

		console.log("******************************************");
		console.log("export from compiled module (Native side):" + this._path);
		var exportsSymbol = WebAssembly.Module.exports(module);
		console.log("   exportsSymbol:" + exportsSymbol);
		for (var f in exportsSymbol)
		{
			var exp = exportsSymbol[f];
			var key = exp.name;
			if( exp.kind == "function")
			{
				this._exportsFunctions[key] = {name: exp.name, kind: exp.kind, ordinal: indexFct, tableIdx: null, fct: null};
				indexFct ++;
				console.log(this._exportsFunctions[key]);
			}
			else if( exp.kind == "table")
			{
				if(key == "__indirect_function_table")
				{
					this._exportsIndirectFunctionsTable = {name: exp.name, kind: exp.kind, ordinal: parseInt(f)};
					console.log(this._exportsIndirectFunctionsTable);
				}
				else
				{
					this._exportsTables[key] = {name: exp.name, kind: exp.kind, ordinal: parseInt(f)};
					console.log(this._exportsTables[key]);
				}
			}
			else
			{
				this._exportsSymbols[key] = {name: exp.name, kind: exp.kind, ordinal: parseInt(f)};
				console.log(this._exportsSymbols[key]);
			}
		}
	}

	extractNativeImplFcts(instance, symbols)
	{
		// Verify memory is not exported by this symbols
		//NYI for standalone executable
		if(instance.exports.memory && !(instance.exports.memory === this._memory.memory) )
		{
			throw internalError(`${this._name} wasm so module with exported memory is not supported`);
		}

		// Get the exported instantiate functions
		console.log("******************************************");
		console.log("export from instanciate module (Native side):" + this._path);
		for (var f in instance.exports)
		{
			var key = f;
			var obj = instance.exports[key];
			if( typeof obj == "function")
			{
				this._exportsFunctions[key].fct = obj;
				console.log(this._exportsFunctions[key]);
			}
			else if( key == "__indirect_function_table")
			{
				console.log("Table length: " + obj.length);
				var i;
				for (i=0; i < obj.length; i++)
				{
					console.log( "Index: " + i + " obj: " + obj.get(i));
				}
				this._exportsIndirectFunctionsTable = obj;
			}
			else
			{
				console.log(obj);
				this._exportsSymbols[key].val = obj;
				console.log(this._exportsSymbols[key]);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////
	// functions helpers  methods
	linkLibrary(linkedLibrary)
	{
		// Insert export symbol of the linked library into the function table of the current library
		if(this._exportsIndirectFunctionsTable != null)
		{
			for (var key in linkedLibrary._exportsFunctions)
			{
				var entryFct = linkedLibrary._exportsFunctions[key];

				var idx  =this._exportsIndirectFunctionsTable.length;
				this._exportsIndirectFunctionsTable.grow(1);
				this._exportsIndirectFunctionsTable.set(idx, entryFct.fct);
				console.log("Map fct: " + entryFct.ordinal + " -> " + idx);
				entryFct.tableIdx = idx;
			}
		}
	}

	// Datatype converstion
	//TODO: have a better semantic on the data type
	_convertParamType(fctName, rawVal)
	{
		if(rawVal == undefined)
			throw new RangeError(`${this._name} can't pass a undefined parameter to Native code`);

		//Get void*
		if(fctName.includes("_pv") && (Math.floor(Number(rawVal)) > 0 && String(Number(rawVal)) == rawVal) )
		{
			if(rawVal == null)
				return 0;

			return parseInt(rawVal);
		}
		//Get a char*
		else if(fctName.includes("_str") && typeof rawVal == "string")
		{
			if(rawVal == null)
				return 0;
			var ptr = this._memory.malloc4JS(rawVal.length + 1); //TODO: Leak
			this._memory.heap_uint8[ptr + rawVal.length] = 0;
			for(let i = 0; i < rawVal.length; ++i)
				this._memory.heap_uint8[ptr + i] = rawVal.charCodeAt(i);
			return ptr;
		}
		//Get int*
		else if(fctName.includes("_pi") && typeof rawVal == "number")
		{
			return parseInt(rawVal);
		}
		//Get an ASCII char
		else if(rawVal.length == 1 && fctName.includes("_c"))
			return rawVal.charCodeAt(0);
		//Get an integer
		else if(fctName.includes("_i") || fctName.includes("_ui"))
			return parseInt(rawVal);
		//Get float
		else if(fctName.includes("_d"))
			return parseFloat(rawVal);

		console.error("Can't get the Datatype parameter for: " + rawVal + "(" + typeof rawVal + ") for :" + fctName);

		return parseInt(rawVal);
	}

	//TODO: have a better semantic on the data type
	_convertReturnFct(fctName, rawVal)
	{
			//Return a char*
			if(fctName.includes("str_"))
			{
				if(rawVal == null)
					return null;
				return this._memory.getStringAt(rawVal);
			}
			//Return a pointer
			else if(fctName.includes("pi_")
					||  fctName.includes("pv_")
					||  fctName.includes("pc_"))
			{
				return parseInt(rawVal);
			}
			//Return a ASCII char
			else if(rawVal <= 255 && fctName.startsWith("c_"))
				return String.fromCharCode(rawVal);
			//Return a int
			else if(fctName.includes("i_") || fctName.includes("ui_"))
				return parseInt(rawVal);
			//Return an floating
			else if(fctName.includes("d_"))
				return parseFloat(rawVal);
			else if(fctName.includes("v_"))
				return undefined;
		throw SyntaxError(`${this._name} Can't get the Datatype return type for: ${rawVal} for: ${fctName}`);
	}

	get dependencies() { return this._dependencies;}

	get exportsSymbols() { return this._exportsSymbols;}
	get exportsFunctions() { return this._exportsFunctions;}

	get importsSymbols() { return this._importsSymbols;}
	get importsFunctions() { return this._importsFunctions;}

	// Use by dlsym in tp.musl JS side
	getTableIdxOf(symbolName)
	{
		var idx = null;
		var fct = this._exportsFunctions[symbolName];
		if (fct != null && fct != undefined)
			idx = fct.tableIdx;

		return idx;
	}
	
	executeFct(fctName)
	{
	  var parameters =  (arguments.length>1)?Array.from(arguments).slice(1) : [];
		var ret;
		if(this._exportsFunctions[fctName] == null)
		{
			throw new ReferenceError(`${this._name} Function('${fctName}') not found in '${this._wasm_so}'`);
		}
		var fct = this._exportsFunctions[fctName].fct;
		var fctParamNumber = fct.length>=0?fct.length:0;
		if(parameters.length != fctParamNumber)
		{
			throw new RangeError(`${this._name} ${fctName} Missmatch between parameters provided(${parameters.length}) and expected(${fctParamNumber})`);
		}

		try
		{
			for(var i = 0; i < fctParamNumber; i++)
			{
				parameters[i] = this._convertParamType(fctName, parameters[i]);
			}
			callerLibrary = this;
			ret = this._convertReturnFct(fctName, fct.apply(null, parameters));
			console.log(`Library.executeFct(${fctName}(${parameters})) => ${ret}`)
			callerLibrary = null;
		}
		catch (error)
		{
			console.error(`Library.executeFct(${fctName}(${parameters})) throw`, error);
			throw error;
		}

		return ret;
	}
}

if(typeof module !== 'undefined' && module.exports)
	module.exports = LibraryBase;


//**************************************************************************************
// library
//Depend on :
//		/js/wasm/library.js
//				LibraryBase
//		/js/helpers/download.js
//				importFile
class Library extends LibraryBase
{
	constructor(arg0, arg1, arg2)
	{
		super(arg0, arg1, arg2);
	}

	////////////////////////////////////////////////////////////////////////
	// Private methods
	_loadWasmJS(imports /*This parameter need to be remove once ld is implemented*/)
	{
		console.log("Loading wasmjs for :" + this._path);
		var scriptTxt = Download.importFile(this._wasm_js);
		if(scriptTxt)
			super.extractSymbolsAndImplFctsJS(imports, scriptTxt)
		else
			throw RangeError(`${this._name} Wasm js not found: ${this._wasm_js}`);
	}

	_loadWasmSO(imports /*There parameters need to be remove from here*/)
	{
		try
		{
			var binary = Download.importFile(this._wasm_so, true);
			if(binary==null)
				throw new RangeError(`${this._name} Wasm module not found: ${this._wasm_so}`);
			if(super.validate(binary) == false)
				throw new RangeError(`${this._name} Not a valid wasm module: ${this._wasm_so}`);
			
			var module = new WebAssembly.Module(binary);
			super.extractNatifSymbols(module, imports);
			var instance = new WebAssembly.Instance(module, imports);
			super.extractNativeImplFcts(instance, imports);
		}
		catch(error)
		{
			console.error(error);
			throw error;
		}
	}
}

const WorkerEvent = {
    eLoad: 'load',
    eGetServerInfo: 'GetServerInfo',
    eGetResponse: 'GetResponse'
}

var moduleLib = null;


onmessage = function(e) {
	var message = e.data;
	console.log("******************************************");
	console.log('Worker: Message received:' + message);
	
	if(message.lenght == 0 || message.lenght > 2) {
		throw 'Worker::onmessage: Bad parameters';
	}
	
	var action = message[0];
	
	console.log('Worker: Action received:' + action);
	switch(action){
    case WorkerEvent.eLoad:
		//TODO: Move import and dependencies inside Library
		var memory = new Memory(2 /*pages*/);
		var imports = {};
		imports.env = {};
		imports.env.memory = memory.memory;
		imports.env.memoryBase = 0;
		imports.env.tableBase = 0;
		imports.env.table = new WebAssembly.Table({ initial: 4, maximum: 100, element: 'anyfunc' });
		imports.table = new WebAssembly.Table({ initial: 5, maximum: 100, element: 'anyfunc' });
		
		
		var libPath = message[1];
		moduleLib = new Library(libPath, memory);
				
		moduleLib.loadModule(imports);
		
		console.log('Worker: Library well loaded');
		postMessage("Worker: Library well loaded");
		
		return;
    case WorkerEvent.eGetServerInfo:
		var res = moduleLib.executeFct("str_getServerInfo_v");

		console.log('Worker: GetServerInfo executed');
		postMessage(res);
		
		return;
    case WorkerEvent.eGetResponse:
		var query = message[1];
		var res = moduleLib.executeFct("str_getResponse_str", query);

		console.log('Worker: GetServerInfo executed');
		postMessage(res);
		
		return;
	}
	
			
	throw 'Unknow action:' + action;
}

class LibraryWorker extends Worker
{
	constructor()
	{
		super('../js/wasm/library.js');
	}
};

if(typeof module !== 'undefined' && module.exports)
	module.exports = Library;
