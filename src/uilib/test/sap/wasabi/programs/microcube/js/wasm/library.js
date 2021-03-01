sap.ui.define(
    [
        "sap/base/Log",
        "sap/firefly/fiori/thirdparty/lodash"
    ],
    function (Log, _) {
        var oResult = {};
        class Memory {
            constructor(nbPages /*, heap_size_pages_max*/ ) {
                this.PAGE_SIZE = 64 * 1024;

                // Equivalent of the break program location:
                // First byte after the available data
                this.m_JS_data_segment = this.PAGE_SIZE;
                this.m_C_data_segment = this.m_JS_data_segment + (nbPages * this.PAGE_SIZE);

                this.m_JS_allocated_bytes = 1; // Reserve NULL adress
                this.m_C_allocated_bytes = 0;

                this.memory = new WebAssembly.Memory({
                    initial: nbPages + 1,
                    /* First page is reserved for the JS side */
                    maximum: nbPages + 1 // td: Manage dynamic page
                });
            }

            // Should be use only from C call
            brk(end_data_segment) {
                var msg = `Memory::brk(${end_data_segment})`;
                if (end_data_segment > this.m_C_data_segment) {

                    var pages_needed = Math.ceil(inc / this.PAGE_SIZE);
                    this.m_memory.grow(pages_needed + 1 /* First page is reserved for the JS side */ );
                    memory = this.m_memory;
                }
                if (end_data_segment != 0)
                    this.m_C_data_segment = end_data_segment;
                Log.info(msg + " => 0");
                return 0;
            }

            //Simple mmap...
            //Should only be call from C
            mmap(length) {
                var msg = `Memory::mmap(${length})`;
                // First page is reserved for JS allocation
                if ((this.m_JS_data_segment + this.m_C_allocated_bytes + length) > this.m_C_data_segment) {
                    //td: this.m_memory.grow
                    return -1;
                }
                this.m_C_allocated_bytes += length;
                Log.info(msg + " => " + this.m_JS_data_segment);
                return this.m_JS_data_segment;
            }

            // JS Memory "stack" filled by JS and consumed by C
            //Should only be call from JS
            // td: Each call is a leak
            malloc4JS(size) {
                var msg = `Memory::malloc4JS(${size})`;
                if (size == 0)
                    throw new RangeError("JS Memory.malloc4JS: Invalid size '0'");
                if ((this.m_JS_allocated_bytes + size) > this.m_JS_data_segment)
                    throw new Error("JS Memory.malloc4JS: No memory left...");
                this.m_JS_allocated_bytes += size;
                var ptr = this.m_JS_allocated_bytes - size;
                Log.info(msg + " => " + ptr);
                return ptr;
            }
            // Heap access helpers.
            get memory() {
                return this.m_memory;
            }
            set memory(memory) {
                this.m_memory = memory
                this.m_heap = this.m_memory.buffer;
                this.m_heap_uint8 = new Uint8Array(this.m_heap);
                this.m_heap_uint32 = new Uint32Array(this.m_heap);
                this.m_C_data_segment = this.m_heap.byteLength;
            }

            get heap_uint8() {
                return this.m_heap_uint8;
            }
            get heap_uint32() {
                return this.m_heap_uint32;
            }

            getCharAt(ptr) {
                return String.fromCharCode(this.m_heap_uint8[ptr]);
            }

            getStringAt(ptr, len = -1) {
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

        oResult.Memory = Memory;

        class Download {
            static get WASMJSON_EXT() {
                return "wasm.json";
            }
            static get WASMJS_EXT() {
                return "wasm.js";
            }
            static get WASMSO_EXT() {
                return "so";
            }
            static get ROOTDIR() {
                return "programs/microcube";
            }
            static get PLATFORM() {
                return "wasm_clang";
            }
            static get BINDIR() {
                return this.ROOTDIR + "/" + this.PLATFORM;
            }

            //td: use cache
            static importFile(url, binary) {
                var data;
                try {
                    var req = new XMLHttpRequest();
                    req.open('GET', url, false); // <-- the 'false' makes it synchronous
                    if (binary == true)
                        req.overrideMimeType('text\/plain; charset=x-user-defined');
                    req.send(null);
                    if (req.status != 200) {
                        return null;
                    }
                    data = req.response;
                    if (binary)
                        return this._stringToArrayBuffer(data);
                } catch (ex) {
                    Log.error("importFile(" + url + "): " + ex.message + ex);
                    return null;
                }
                return data;
            }

            static importWasmMetadata(libname) {
                let url = libname + "." + this.WASMJSON_EXT;
                let metadata = this.importFile(url)
                if (metadata == null)
                    throw new ReferenceError("No metadata file found");
                return JSON.parse(metadata);
            }

            static _stringToArrayBuffer(str) {
                var buf = new ArrayBuffer(str.length);
                var bufView = new Uint8Array(buf);

                for (var i = 0, strLen = str.length; i < strLen; i++) {
                    bufView[i] = str.charCodeAt(i);
                }

                return buf;
            }
        }

        oResult.Download;



        //**************************************************************************************
        // library_base
        //Use by libc.wasm.js to identify the caller library
        //   inityalize in this class
        //   should be set before all call to the JS call in libc.wasm.js
        //td: find a better way than a global var...
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

        class LibraryBase {
            constructor(arg0, arg1, arg2) {
                if (arg2 === undefined) {
                    if (arg0.indexOf(Download.ROOTDIR) != -1)
                        this._path = arg0;
                    else
                        this._path = Download.ROOTDIR + arg0;
                    this._memory = arg1;
                    this._folder = this._path.substring(0, this._path.lastIndexOf("/"));
                } else {
                    this._folder = arg0;
                    this._path = arg0 + "/" + arg1;
                    this._memory = arg2;
                }
                this._name = this._path.substring(this._path.lastIndexOf("/") + 1);
                this._importsFunctions = {
                    env: {}
                };
                this._importsSymbols = {
                    env: {}
                };

                this._exportsFunctions = {}; //fctname:{fct:therealfunction};
                this._exportsIndirectFunctionsTable = null;

                this._exportsTables = {};
                this._exportsSymbols = {};
                this._exportsSymbols_wasm_js = null;

                this._wasm_js = null;
                this._wasm_so = null;
                this._version = null;
                this._dependencies = {};
            }


            //     For the moment the default behavior is RTLD_GLOBAL | RTLD_NOW
            loadModule(imports) {
                var that = this;
                Log.info("********************************************************************************************************");
                Log.info("Start to load Library :" + that._path);
                // Load metadata first
                that._loadMetadata();
                // Load dependencies coming from metadata defintion
                // Done first because this current code contains the Dyn Loader (td: need to be separate)
                Log.info("******************************************");
                Log.info("Start to load dependencies");
                var aLib = [];
                for (var dep in this._dependencies) {
                    let libPath = this._dependencies[dep];
                    Log.info(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                    Log.info(libPath);
                    aLib.push({
                        lib: new Library(libPath),
                        imp: imports,
                        path: libPath
                    });


                }
                return Promise.all(
                    _.map(aLib, function (oL) {
                        Log.info("Load: " + oL.path);
                        return oL.lib.loadModule(oL.imp).then(function () {
                            Log.info("Loaded: " + oL.path);
                        })
                    })
                ).then(function () {
                    Log.info("Finish to load dependencies");

                    if (that._wasm_js)
                        that._loadWasmJS(imports);
                    if (that._wasm_so)
                        return that._loadWasmSO(imports);
                    return null;
                }).then(function () {
                    Log.info("******************************************");
                    Log.info("Add symbols to table");
                    //this.linkLibrary(this);

                    Log.info("******************************************");
                    Log.info("Add symbols of dependencies to table");
                    for (var dep in that._dependencies) {
                        let libName = that._dependencies[dep];
                        Log.info(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                        Log.info(libName);
                        that.linkLibrary(dep);
                        Log.info("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
                    }
                    return that;
                }).catch(function (e) {
                    Log.error(e);
                    throw e;
                });
            }
            ////////////////////////////////////////////////////////////////////////
            // Private methods
            _loadMetadata() {
                Log.info("******************************************");
                Log.info("Loading metadata for :" + this._path);
                let metadata = Download.importWasmMetadata(this._path);
                if (!metadata.version)
                    throw new ReferenceError(`Missing version field from metadata of '${this._path}'`)
                this._version = metadata.version;
                if (metadata.wasm_js)
                    this._wasm_js = Download.ROOTDIR + metadata.wasm_js;
                if (metadata.wasm_so)
                    this._wasm_so = Download.ROOTDIR + metadata.wasm_so;
                if (metadata.libs)
                    this._dependencies = metadata.libs;
                Log.info(metadata);
            }

            // return boolean
            validate(arrayBuffer) {
                return WebAssembly.validate(arrayBuffer);
            }

            extractSymbolsAndImplFctsJS(imports, jsTxt) {
                Log.info("********************************************************************************************************");
                Log.info("export from module (JS side):" + this._path);

                var jssymbols = {};
                var getJSExports = undefined;
                //td: eval the JS only once by js... or find a better way than eval.apply
                try {
                    if (typeof module !== 'undefined' && module.exports)
                        getJSExports = require("../../../../" + this._wasm_js);
                    else {
                        var b = eval.call(this, jsTxt);
                        getJSExports = window.getJSExports;
                    }
                } catch (e) {
                    throw RangeError(`${this._name} Error during evauation of ${this._wasm_js}`, e);
                }
                if (typeof getJSExports == 'undefined')
                    throw ReferenceError(`${this._name} Missing defintion of the function getJSExports(exports) in ${this._wasm_js}`)
                var exports = {};
                getJSExports(exports);
                var jssymbols = exports[this._name];
                if (!jssymbols)
                    throw ReferenceError(`${this._name} Missing initialisation of exports.${this._name} in ${this._wasm_js}:fct(${getJSExports})`)

                for (var group in jssymbols) {
                    Log.info(group, jssymbols[group]);
                }

                // td: this should be manage by the Dyn Linker
                // Resolve import module with wasm.js exportsSymbols
                for (var group in jssymbols) {
                    for (var f in jssymbols[group])
                        imports.env[f] = jssymbols[group][f];
                }
                this._exportsSymbols_wasm_js = jssymbols;
            }

            extractNatifSymbols(module, imports) {
                Log.info("******************************************");
                Log.info("customSectionsName from compiled module (Native side):" + this._path);
                let customSectionsName = WebAssembly.Module.customSections(module, "name");
                for (var f in customSectionsName) {
                    Log.info(customSectionsName[f]);
                }

                // td: Need to verify specifications :
                //	export fct list match index of function(eq fct ptr)) in term of order and base index
                //  index is based starting after the last imported function... :?
                var indexFct = 1; // Start to 1, 0 is null ptr

                Log.info("******************************************");
                Log.info("import from compiled module (Native side):" + this._path);
                let importsSymbols = WebAssembly.Module.imports(module);
                for (var f in importsSymbols) {
                    Log.info(importsSymbols[f]);
                    if (importsSymbols[f].kind == "function") {
                        this._importsFunctions["env"][f] = importsSymbols[f];
                        indexFct++;
                    } else
                        this._importsSymbols["env"][f] = importsSymbols[f];
                }

                Log.info("******************************************");
                Log.info("export from compiled module (Native side):" + this._path);
                var exportsSymbol = WebAssembly.Module.exports(module);
                Log.info("   exportsSymbol:" + exportsSymbol);
                for (var f in exportsSymbol) {
                    var exp = exportsSymbol[f];
                    var key = exp.name;
                    if (exp.kind == "function") {
                        this._exportsFunctions[key] = {
                            name: exp.name,
                            kind: exp.kind,
                            ordinal: indexFct,
                            tableIdx: null,
                            fct: null
                        };
                        indexFct++;
                        Log.info(this._exportsFunctions[key]);
                    } else if (exp.kind == "table") {
                        if (key == "__indirect_function_table") {
                            this._exportsIndirectFunctionsTable = {
                                name: exp.name,
                                kind: exp.kind,
                                ordinal: parseInt(f)
                            };
                            Log.info(this._exportsIndirectFunctionsTable);
                        } else {
                            this._exportsTables[key] = {
                                name: exp.name,
                                kind: exp.kind,
                                ordinal: parseInt(f)
                            };
                            Log.info(this._exportsTables[key]);
                        }
                    } else {
                        this._exportsSymbols[key] = {
                            name: exp.name,
                            kind: exp.kind,
                            ordinal: parseInt(f)
                        };
                        Log.info(this._exportsSymbols[key]);
                    }
                }
            }

            extractNativeImplFcts(instance, symbols) {
                // Verify memory is not exported by this symbols
                //NYI for standalone executable
                if (instance.exports.memory && !(instance.exports.memory === this._memory.memory)) {
                    throw internalError(`${this._name} wasm so module with exported memory is not supported`);
                }

                // Get the exported instantiate functions
                Log.info("******************************************");
                Log.info("export from instanciate module (Native side):" + this._path);
                for (var f in instance.exports) {
                    var key = f;
                    var obj = instance.exports[key];
                    if (typeof obj == "function") {
                        this._exportsFunctions[key].fct = obj;
                        Log.info(this._exportsFunctions[key]);
                    } else if (key == "__indirect_function_table") {
                        Log.info("Table length: " + obj.length);
                        var i;
                        for (i = 0; i < obj.length; i++) {
                            Log.info("Index: " + i + " obj: " + obj.get(i));
                        }
                        this._exportsIndirectFunctionsTable = obj;
                    } else {
                        Log.info(obj);
                        this._exportsSymbols[key].val = obj;
                        Log.info(this._exportsSymbols[key]);
                    }
                }
            }

            ////////////////////////////////////////////////////////////////////////
            // functions helpers  methods
            linkLibrary(linkedLibrary) {
                // Insert export symbol of the linked library into the function table of the current library
                if (this._exportsIndirectFunctionsTable != null) {
                    for (var key in linkedLibrary._exportsFunctions) {
                        var entryFct = linkedLibrary._exportsFunctions[key];

                        var idx = this._exportsIndirectFunctionsTable.length;
                        this._exportsIndirectFunctionsTable.grow(1);
                        this._exportsIndirectFunctionsTable.set(idx, entryFct.fct);
                        Log.info("Map fct: " + entryFct.ordinal + " -> " + idx);
                        entryFct.tableIdx = idx;
                    }
                }
            }

            // Datatype converstion
            //td: have a better semantic on the data type
            _convertParamType(fctName, rawVal) {
                if (rawVal == undefined)
                    throw new RangeError(`${this._name} can't pass a undefined parameter to Native code`);

                //Get void*
                if (fctName.includes("_pv") && (Math.floor(Number(rawVal)) > 0 && String(Number(rawVal)) == rawVal)) {
                    if (rawVal == null)
                        return 0;

                    return parseInt(rawVal);
                }
                //Get a char*
                else if (fctName.includes("_str") && typeof rawVal == "string") {
                    if (rawVal == null)
                        return 0;
                    var ptr = this._memory.malloc4JS(rawVal.length + 1); //td: Leak
                    this._memory.heap_uint8[ptr + rawVal.length] = 0;
                    for (let i = 0; i < rawVal.length; ++i)
                        this._memory.heap_uint8[ptr + i] = rawVal.charCodeAt(i);
                    return ptr;
                }
                //Get int*
                else if (fctName.includes("_pi") && typeof rawVal == "number") {
                    return parseInt(rawVal);
                }
                //Get an ASCII char
                else if (rawVal.length == 1 && fctName.includes("_c"))
                    return rawVal.charCodeAt(0);
                //Get an integer
                else if (fctName.includes("_i") || fctName.includes("_ui"))
                    return parseInt(rawVal);
                //Get float
                else if (fctName.includes("_d"))
                    return parseFloat(rawVal);

                Log.error("Can't get the Datatype parameter for: " + rawVal + "(" + typeof rawVal + ") for :" + fctName);

                return parseInt(rawVal);
            }

            //td: have a better semantic on the data type
            _convertReturnFct(fctName, rawVal) {
                //Return a char*
                if (fctName.includes("str_")) {
                    if (rawVal == null)
                        return null;
                    return this._memory.getStringAt(rawVal);
                }
                //Return a pointer
                else if (fctName.includes("pi_") ||
                    fctName.includes("pv_") ||
                    fctName.includes("pc_")) {
                    return parseInt(rawVal);
                }
                //Return a ASCII char
                else if (rawVal <= 255 && fctName.startsWith("c_"))
                    return String.fromCharCode(rawVal);
                //Return a int
                else if (fctName.includes("i_") || fctName.includes("ui_"))
                    return parseInt(rawVal);
                //Return an floating
                else if (fctName.includes("d_"))
                    return parseFloat(rawVal);
                else if (fctName.includes("v_"))
                    return undefined;
                throw SyntaxError(`${this._name} Can't get the Datatype return type for: ${rawVal} for: ${fctName}`);
            }

            get dependencies() {
                return this._dependencies;
            }

            get exportsSymbols() {
                return this._exportsSymbols;
            }
            get exportsFunctions() {
                return this._exportsFunctions;
            }

            get importsSymbols() {
                return this._importsSymbols;
            }
            get importsFunctions() {
                return this._importsFunctions;
            }

            // Use by dlsym in tp.musl JS side
            getTableIdxOf(symbolName) {
                var idx = null;
                var fct = this._exportsFunctions[symbolName];
                if (fct != null && fct != undefined)
                    idx = fct.tableIdx;

                return idx;
            }

            executeFct(fctName) {
                var parameters = (arguments.length > 1) ? Array.from(arguments).slice(1) : [];
                var ret;
                if (this._exportsFunctions[fctName] == null) {
                    throw new ReferenceError(`${this._name} Function('${fctName}') not found in '${this._wasm_so}'`);
                }
                var fct = this._exportsFunctions[fctName].fct;
                var fctParamNumber = fct.length >= 0 ? fct.length : 0;
                if (parameters.length != fctParamNumber) {
                    throw new RangeError(`${this._name} ${fctName} Missmatch between parameters provided(${parameters.length}) and expected(${fctParamNumber})`);
                }
                try {
                    for (var i = 0; i < fctParamNumber; i++) {
                        parameters[i] = this._convertParamType(fctName, parameters[i]);
                    }
                    callerLibrary = this;
                    ret = this._convertReturnFct(fctName, fct.apply(null, parameters));
                    Log.info(`Library.executeFct(${fctName}(${parameters})) => ${ret}`)
                    callerLibrary = null;
                } catch (error) {
                    Log.error(`Library.executeFct(${fctName}(${parameters})) throw`, error);
                    throw error;
                }

                return ret;
            }
        }

        oResult.LibraryBase;


        //**************************************************************************************
        // library
        //Depend on :
        //		/js/wasm/library.js
        //				LibraryBase
        //		/js/helpers/download.js
        //				importFile
        class Library extends LibraryBase {
            constructor(arg0, arg1, arg2) {
                super(arg0, arg1, arg2);
            }

            ////////////////////////////////////////////////////////////////////////
            // Private methods
            _loadWasmJS(imports /*This parameter need to be remove once ld is implemented*/ ) {
                Log.info("Loading wasmjs for :" + this._path);
                var scriptTxt = Download.importFile(this._wasm_js);
                if (scriptTxt)
                    super.extractSymbolsAndImplFctsJS(imports, scriptTxt)
                else
                    throw RangeError(`${this._name} Wasm js not found: ${this._wasm_js}`);
            }

            _loadWasmSO(imports /*There parameters need to be remove from here*/ ) {
                var extractNatifSymbols = super.extractNatifSymbols.bind(this);
                var extractNativeImplFcts = super.extractNativeImplFcts.bind(this);
                var binary = Download.importFile(this._wasm_so, true);
                if (binary == null)
                    throw new RangeError(`${this._name} Wasm module not found: ${this._wasm_so}`);
                if (super.validate(binary) == false)
                    throw new RangeError(`${this._name} Not a valid wasm module: ${this._wasm_so}`);
                return WebAssembly.instantiate(binary, imports).then(function (oExecutable) {
                    extractNatifSymbols(oExecutable.module, imports);
                    extractNativeImplFcts(oExecutable.instance, imports);
                }).catch(function (error) {
                    Log.error(error);
                    throw error;
                });
            }
        }
        oResult.Library = Library;
        return oResult;
    }
);