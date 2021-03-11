(function(){
 //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Microcube_worker.js
var isLoaded = false;

var WorkerEvent = {
    eLoad: 'load',
    eGetServerInfo: 'GetServerInfo',
    eGetResponse: 'GetResponse'
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// WASI_API.js

// https://docs.rs/wasi/0.10.2+wasi-snapshot-preview1/wasi/wasi_snapshot_preview1/index.html

// Members
moduleInstanceExports = null;

const  WASI_CLOCKID_REALTIME                          =  0;
const  WASI_CLOCKID_MONOTONIC                         =  1;
const  WASI_CLOCKID_PROCESS_CPUTIME_ID                =  2;
const  WASI_CLOCKID_THREAD_CPUTIME_ID                 =  3;

function clock_res_realtime () {
			return BigInt(1e6);
};

function clock_res_monotonic() {
			return BigInt(1e3);
};

const clock_res_process = clock_res_monotonic;
const clock_res_thread = clock_res_monotonic;

function clock_time_realtime () {
			return BigInt(Date.now()) * BigInt(1e6);
};

function clock_time_monotonic () {
			const t = performance.now();
			const s = Math.trunc(t);
			const ms = Math.floor((t - s) * 1e3);

			return (BigInt(s) * BigInt(1e9)) + (BigInt(ms) * BigInt(1e6));
};

const clock_time_process = clock_time_monotonic;
const clock_time_thread = clock_time_monotonic;

// env : { [key: string]: string | undefined };
env = {};
//

fs_Path2Data = new Map();

fds = [
		{
			vpath:"/dev/stdin",
			offset:BigInt(0),
			data:null,
		},
		{
			vpath:"/dev/stdout",
			offset:BigInt(0),
			data:null,
		},
		{
			vpath:"/dev/stderr",
			offset:BigInt(0),
			data:null,
		},
		{
			vpath:".",
			offset:BigInt(0),
			data:null,
		},
	];

function convertWAsmStr2JSStr(str_ptr, memory)
{
	const heap = new Uint8Array(memory.buffer);
	var str = "";
	for (var i = str_ptr; heap[i] != 0; ++i)
		str += String.fromCharCode(heap[i]);
	return str;
}

function convertJSStr2WAsm(js_str, module)
{
	const heap = new Uint8Array(module.instance.exports.memory.buffer);

	var uint8array = new TextEncoder("utf-8").encode(js_str);
	let size = uint8array.length;
	
	var wasm_str = module.instance.exports.malloc(size+1);
	
	for (var i = 0; i < size; i++) {
		heap[wasm_str+i] = uint8array[i];
	}
	heap[wasm_str+size] = 0;
	
	return wasm_str;
}

function getModuleMemoryDataView() {
	return new DataView(moduleInstanceExports.memory.buffer);
}
function setModuleInstance(instance) {
	moduleInstanceExports = instance.exports;
}
function getModuleInstance() {
	return moduleInstanceExports;
}



var WASI_API = {
	// WASI constants
	WASI_ESUCCESS: WASI_ESUCCESS = 0,
	WASI_EBADF : WASI_EBADF = 8,
	WASI_EINVAL : WASI_EINVAL = 28,
	WASI_ENOSYS : WASI_ENOSYS = 52,
	
	WASI_STDOUT_FILENO : WASI_STDOUT_FILENO = 1,
	WASI_STDERR_FILENO : WASI_STDERR_FILENO = 2,
	
	WASI_SEEK_START : WASI_SEEK_START = 0,
	WASI_SEEK_CUR : WASI_SEEK_CUR = 1,
	WASI_SEEK_END : WASI_SEEK_END = 2,
	
	WASI_PREOPENTYPE_DIR : WASI_PREOPENTYPE_DIR = 0,

	// WASI API
	
	//*************************************************************
	// wasabi specific
	wasabi_initFS : async function(rootFS, paths) {
		for (let i=0; i<paths.length; i++)  {
			vpath = paths[i];
			if(rootFS != null && rootFS != "" )
				path = rootFS + vpath;
			else
				path = vpath;
			const response = await fetch(path);
			if (response.status >= 400 && response.status < 600) {
				response.text().then(function (text) {
					throw new Error("Bad response from server response.status='"+response.status+"' " + text + "'");
				  });
			  }
			arrayBuffer = await response.arrayBuffer();
			var uint8View = new Uint8Array(arrayBuffer);
			
			if(vpath.substring(0, 1) == "/")
				vpath = vpath.substring(1);
			fs_Path2Data.set(vpath, uint8View);
			console.log("WASI FileSystem: [" +rootFS + "," + vpath + "] loaded from '" + path + "' (" + uint8View.length + " bytes)");
		}
	},
	wasabi_initEnv : async function(environ) {
		env = environ ? environ : {} ;
	},
	
	log : log = function(msg) {
		//console.log("WASI-"+arguments.callee.caller.name + ":" + msg);
	},
	error : error = function(msg) {
		console.error("WASI: " + msg);
	},
	
	//*************************************************************
	// stdout and stderr 
	wasabi_log : wasabi_log = function(msg) {
		if(typeof document !== 'undefined') {
			document.getElementById('log').innerHTML += msg.replace(/\n( *)/g, function (match, p1) {
				return '<br>' + '&nbsp;'.repeat(p1.length);
			});
		}
		console.log(msg);
	},
	wasabi_error : wasabi_error = function(msg) {
		if(typeof document !== 'undefined') {
			document.getElementById('log').innerHTML += msg.replace(/\n( *)/g, function (match, p1) {
				return '<br>' + '&nbsp;'.repeat(p1.length);
			});
		}
		console.error(msg);
	},
	
	//*************************************************************
	// process
	proc_exit: function(rval) {
		log(Array.prototype.slice.call(arguments));
		return WASI_ENOSYS;
	},
	
	//*************************************************************
	// path
	path_open: function(dirfd, dirflags, path_ptr, path_len, oflags, fs_rights_base, fs_rights_inheriting, fs_flags, fd) {
		log(Array.prototype.slice.call(arguments));
		vpath  = convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory);
		log("vpath:'" + vpath +"'");
		
		const entry = fds[dirfd];
		if (!entry) {
			error("Invalid dirfd=" + dirfd);
			return WASI_EBADF;
		} else if (!entry.vpath) {
			error("No vpath for dirfd="+dirfd);
			return WASI_EINVAL;
		}

		const text = new TextDecoder();
		const data = new Uint8Array(moduleInstanceExports.memory.buffer, path_ptr, path_len);

		size = fds.length;
		offset = BigInt(0);
		const opened_fd = fds.push({vpath, offset}) - 1;
		const view = new DataView(moduleInstanceExports.memory.buffer);
		view.setUint32(fd, opened_fd, true);
			
		return WASI_ESUCCESS;
	},
	path_filestat_get: function(fd, flags, path_ptr, path_len, buf) {
		log(Array.prototype.slice.call(arguments));
		log("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");
		return WASI_ENOSYS;
	},
	path_unlink_file: function(fd, path_ptr, path_len) {
		log(Array.prototype.slice.call(arguments));
		log("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");
		return WASI_ENOSYS;
	},
	poll_oneoff: function(in_, out, nsubscriptions, nevents) {
		log(Array.prototype.slice.call(arguments));
		return WASI_ENOSYS;
	},
	
	
	//*************************************************************
	// file descriptor
	fd_sync: function(fd) {
		log(Array.prototype.slice.call(arguments));
		return WASI_ENOSYS;
	},
	fd_seek: function(fd, offset, whence, newoffset_ptr) {
		log(Array.prototype.slice.call(arguments));
		const entry = fds[fd];
		if (!entry) {
			error("Invalid fd=" + fd);
			return WASI_EBADF;
		}
		if(whence == WASI_SEEK_START)
		    entry.offset = offset;
		else if(whence == WASI_SEEK_CUR) {
		    entry.offset += offset;
		} else if(whence == WASI_SEEK_END) {
		    entry.offset += offset;
		} else {
			error("fd_seek invalid whence");
			return WASI_EINVAL;
		}
		
		if(entry.offset < 0)
			entry.offset = BigInt(0);
		
		const view = new DataView(moduleInstanceExports.memory.buffer);
		view.setBigUint64(newoffset_ptr, BigInt(entry.offset), true);

		return WASI_ESUCCESS;
	},
	fd_write: function(fd, iovs_ptr, iovs_len, nwritten_ptr) {
		log(Array.prototype.slice.call(arguments));
		const entry = fds[fd];
		if (!entry) {
			error("Invalid fd=" + fd);
			return WASI_EBADF;
		} else if (!entry.vpath) {
			error("No vpath for fd="+fd);
			return WASI_EINVAL;
		}
		
		var view = getModuleMemoryDataView();

		var written = 0;
		var bufferBytes = [];                   

		function getiovs(iovs_ptr, iovs_len) {
			// iovs_ptr* -> [iov, iov, ...]
			// __wasi_ciovec_t {
			//   void* buf,
			//   size_t buf_len,
			// }
			var buffers = Array.from({ length: iovs_len }, function (_, i) {
				   var ptr = iovs_ptr + i * 8;
				   var buf = view.getUint32(ptr, !0);
				   var bufLen = view.getUint32(ptr + 4, !0);

				   return new Uint8Array(moduleInstanceExports.memory.buffer, buf, bufLen);
				});

			return buffers;
		}

		var buffers = getiovs(iovs_ptr, iovs_len);
		function writev(iov) {
			for (var b = 0; b < iov.byteLength; b++) {
			   bufferBytes.push(iov[b]);
			}
			written += b;
		}

		buffers.forEach(writev);

		if (fd === WASI_STDOUT_FILENO) wasabi_log(String.fromCharCode.apply(null, bufferBytes));    
		else if (fd === WASI_STDERR_FILENO) wasabi_error(String.fromCharCode.apply(null, bufferBytes));    
		else {
			error("fd_write NYI");
			return WASI_ENOSYS;
		}

		view.setUint32(nwritten_ptr, written, !0);

		return WASI_ESUCCESS;
	},
	fd_read: function(fd, iovs_ptr, iovs_len, nread_ptr) {
		log(Array.prototype.slice.call(arguments));
		const entry = fds[fd];
		if (!entry) {
			error("Invalid fd="+fd);
			return WASI_EBADF;
		} else if (!entry.vpath) {
			error("No vpath for fd="+fd);
			return WASI_EINVAL;
		}
		
		if(!entry.data) {
			entry.data = fs_Path2Data.get(entry.vpath);
			if(!entry.data)
			{
				error("Please register your file '" + entry.vpath + "' by calling WASI_API::wasabi_initFS()");
				return WASI_EBADF;
			}
			entry.offset = BigInt(0);
		}

		const view = new DataView(moduleInstanceExports.memory.buffer);

		let nread = 0;
		for (let i = 0; i < iovs_len; i++) {
			const data_ptr = view.getUint32(iovs_ptr, true);
			iovs_ptr += 4;

			const data_len = view.getUint32(iovs_ptr, true);
			iovs_ptr += 4;

			const data = new Uint8Array(moduleInstanceExports.memory.buffer, data_ptr, data_len);
			for(let j =0; j < data_len && entry.offset < entry.data.length ; j++) {
				data[j] = entry.data[entry.offset++];
				nread++;
			}
		}

		view.setUint32(nread_ptr, nread, true);

		return WASI_ESUCCESS;
	},
	fd_close: function(fd) {
		log(Array.prototype.slice.call(arguments));
		const entry = fds[fd];
		if (!entry) {
			error('Invalid fd='+fd);
			return WASI_EBADF;
		}
		fds.splice(fd);
		return WASI_ESUCCESS;
	},
	fd_filestat_get: function(fd, buf_ptr) {
		log(Array.prototype.slice.call(arguments));
		return WASI_ENOSYS;
	},
	fd_fdstat_set_flags: function(fd, flags) {
		log(Array.prototype.slice.call(arguments));
	},
	fd_fdstat_get: function(fd, stat_ptr) {
		log(Array.prototype.slice.call(arguments));
		var view = getModuleMemoryDataView();

		view.setUint8(stat_ptr, fd);
		view.setUint16(stat_ptr + 2, 0, !0);
		view.setUint16(stat_ptr + 4, 0, !0);

		function setBigUint64(byteOffset, value, littleEndian) {
			var lowWord = value;
			var highWord = 0;

			view.setUint32(littleEndian ? 0 : 4, lowWord, littleEndian);
			view.setUint32(littleEndian ? 4 : 0, highWord, littleEndian);
		}

		setBigUint64(stat_ptr + 8, 0, !0);
		setBigUint64(stat_ptr + 8 + 8, 0, !0);

		return WASI_ESUCCESS;
	},
	fd_prestat_get: function(fd, buf_out) {
		log(Array.prototype.slice.call(arguments));
		const entry = fds[fd];
		if (!entry) {
			error("Invalid fd=" + fd);
			return WASI_EBADF;
		} else if (!entry.vpath) {
			error("No vpath for fd="+fd);
			return WASI_EBADF;
		}
		log("vpath:'" + entry.vpath +"'");

		const view = new DataView(moduleInstanceExports.memory.buffer);
		view.setUint8(buf_out, WASI_PREOPENTYPE_DIR);
		view.setUint32(buf_out + 4, new TextEncoder().encode(entry.vpath).byteLength, true);
		return WASI_ESUCCESS;
	},
	fd_prestat_dir_name: function(fd, path_ptr, path_len) {
		log(Array.prototype.slice.call(arguments));
		const entry = fds[fd];
		if (!entry) {
			return WASI_EBADF;
		}
		if (!entry.vpath) {
			return WASI_EBADF;
		}
		log("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");

		const data = new Uint8Array(moduleInstanceExports.memory.buffer, path_ptr, path_len);
		data.set(new TextEncoder().encode(entry.vpath));

		return WASI_ESUCCESS;
	},
	
	
	//*************************************************************
	// var env
	environ_sizes_get: function(environ_size, environ_buf_size) {
		log(Array.prototype.slice.call(arguments));
		const entries = Object.entries(env);
		const text = new TextEncoder();
		var view = getModuleMemoryDataView();

		view.setUint32(environ_size, entries.length, !0);
		view.setUint32(environ_buf_size, entries.reduce(function(acc, [key, value]) {
			return acc + text.encode(`${key}=${value}\0`).length;
		}, 0), true);

		return WASI_ESUCCESS;
	},
	environ_get: function(environ_ptr, environ_buf_ptr) {
		log(Array.prototype.slice.call(arguments));
		const entries = Object.entries(env);
		const text = new TextEncoder();
		const heap = new Uint8Array(this.memory.buffer);
		var view = getModuleMemoryDataView();

		for (let [key, value] of entries) {
			view.setUint32(environ_ptr, environ_buf_ptr, true);
			environ_ptr += 4;

			const data = text.encode(`${key}=${value}\0`);
			heap.set(data, environ_buf_ptr);
			environ_buf_ptr += data.length;
		}

		return WASI_ESUCCESS;
	},
	
	//*************************************************************
	// Clock
	
	clock_res_get: function (clock_id, resolution_out) {
		var view = getModuleMemoryDataView();

		switch (clock_id) {
			case WASI_CLOCKID_REALTIME:
				view.setBigUint64(resolution_out, clock_res_realtime(), true);
				break;

			case WASI_CLOCKID_MONOTONIC:
				view.setBigUint64(resolution_out, clock_res_monotonic(), true);
				break;

			case WASI_CLOCKID_PROCESS_CPUTIME_ID:
				view.setBigUint64(resolution_out, clock_res_process(), true);
				break;

			case WASI_CLOCKID_THREAD_CPUTIME_ID:
				view.setBigUint64(resolution_out, clock_res_thread(), true);
				break;

			default:
				return WASI_INVAL;

		}

		return WASI_ESUCCESS;
	},
	clock_time_get: function(clock_id, precision, time) {
		log(Array.prototype.slice.call(arguments));
		var view = getModuleMemoryDataView();
		switch (clock_id) {
			case WASI_CLOCKID_REALTIME:
				view.setBigUint64(time, clock_time_realtime(), true);
				break;

			case WASI_CLOCKID_MONOTONIC:
				view.setBigUint64(time, clock_time_monotonic(), true);
				break;

			case WASI_CLOCKID_PROCESS_CPUTIME_ID:
				view.setBigUint64(time, clock_time_process(), true);
				break;

			case WASI_CLOCKID_THREAD_CPUTIME_ID:
				view.setBigUint64(time, clock_time_thread(), true);
				break;

			default:
				return WASI_INVAL;

		}
		return WASI_ESUCCESS;
	},
};
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
moduleWASI = null;

const indexMsgId = 0;
const indexMsgAction = 1;
const indexMsgParam = 2;

onmessage = function(e) {
	var message = e.data;
	if(message.length != 3) {
		throw 'Worker::onmessage: Expect 3 parameters';
	}

	var ID = message[indexMsgId];
	var action = message[indexMsgAction];
	var param = message[indexMsgParam];

	console.log("******************************************");
	console.log("Worker: Message received: ID '" + ID + "' Action:'" + action + "'");
	console.log("Worker: Message received: param='" + param.substring(0, 150) + "'");
	

	switch(action){
    case WorkerEvent.eLoad:
		if(isLoaded == true){
			console.error("Worker: Already loaded");
			postMessage([ID, action, {error: "Already loaded"}]);
			return;
		}

		var importObject =
		{
			wasi_snapshot_preview1: WASI_API,
			env: {},
			js : {mem: new WebAssembly.Memory({initial: 2,maximum: 100})}
		};
		WebAssembly.instantiateStreaming(fetch("./InA_Interpreter.wasm"), importObject).then(module =>
		{
			console.log('Worker: Library well loaded');
			moduleWASI = module;
			setModuleInstance(module.instance);
			
			filesystem = ["/resources/response_getSerververInfo.json",
						 "/resources/response_getResponse_Metadat_expand_cube_catalog.json",
						 "/resources/sqlite/efashion/efashion.db"];
			WASI_API.wasabi_initFS(param, filesystem).then(() => 
			{
				if(getModuleInstance()._start)
				{
					console.log('Worker: execute _start');
					getModuleInstance()._start();
				} else
				{
					console.log('Worker: no _start entry point');
				}
				postMessage([ID, WorkerEvent.eLoad, "Worker: Library well loaded"]);
				isLoaded = true;
			});
		}).catch(error=>{
			console.log('Worker: Library not well loaded: ', error),
			console.log("stack" + error.stack),
			postMessage([ID, WorkerEvent.eLoad, error])
		});

		return;
    case WorkerEvent.eGetServerInfo:
		if(isLoaded == false){
			console.error("Worker: Not loaded");
			postMessage([ID, WorkerEvent.eGetServerInfo, {error: "Not loaded"}]);
			return;
		}

		try {
			var wasmRes = moduleWASI.instance.exports.json_getServerInfo();
			var res = convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);

			console.log([WorkerEvent.eGetServerInfo, 'Worker: GetServerInfo executed']);
			postMessage([ID, WorkerEvent.eGetServerInfo, res]);
		} catch(error) {
			console.log('Worker: eGetServerInfo error: ', error);
			console.log("stack" + error.stack),
			postMessage([ID, WorkerEvent.eGetServerInfo, error]);
		}
		
		return;
    case WorkerEvent.eGetResponse:
		if(isLoaded == false){
			console.error("Worker: Not loaded");
			postMessage([ID, WorkerEvent.eGetResponse, {error: "Not loaded"}]);
			return;
		}

		try {
			var queryJS = message[indexMsgParam];
			
			var queryWAsm = convertJSStr2WAsm(queryJS, moduleWASI);
			var wasmRes = moduleWASI.instance.exports.json_getResponse_json(queryWAsm);
			moduleWASI.instance.exports.free(queryWAsm);
			
			var res = convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);

			console.log([WorkerEvent.eGetResponse, 'Worker: GetServerInfo executed']);
			postMessage([ID, WorkerEvent.eGetResponse, res]);
		} catch(error) {
			console.log('Worker: eGetResponse error: ', error);
			console.log("stack" + error.stack),
			postMessage([ID, WorkerEvent.eGetResponse, error]);
		}
		
		return;
	}
	
	postMessage([ID, action, new Error('Unknow action:' + action)]);
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}());