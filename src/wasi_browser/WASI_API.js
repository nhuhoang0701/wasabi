//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// WASI_API.js

// https://github.com/WebAssembly/WASI/blob/main/phases/snapshot/docs.md
// https://docs.rs/wasi/0.10.2+wasi-snapshot-preview1/wasi/wasi_snapshot_preview1/index.html


let DATA_ADDR = null;
let sleeping = false;

let WASI_API = {
	//////////////////////////////////////////////////////////////
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

		
	WASI_CLOCKID_REALTIME : WASI_CLOCKID_REALTIME =  0,
	WASI_CLOCKID_MONOTONIC : WASI_CLOCKID_MONOTONIC =  1,
	WASI_CLOCKID_PROCESS_CPUTIME_ID : WASI_CLOCKID_PROCESS_CPUTIME_ID=  2,
	WASI_CLOCKID_THREAD_CPUTIME_ID : WASI_CLOCKID_THREAD_CPUTIME_ID=  3,

	WASI_FILETYPE_UNKNOWN : WASI_FILETYPE_UNKNOWN=  0,
	WASI_FILETYPE_BLOCK_DEVICE : WASI_FILETYPE_BLOCK_DEVICE=  1,
	WASI_FILETYPE_CHARACTER_DEVICE  :WASI_FILETYPE_CHARACTER_DEVICE=  2,
	WASI_FILETYPE_DIRECTORY : WASI_FILETYPE_DIRECTORY=  3,
	WASI_FILETYPE_REGULAR_FILE : WASI_FILETYPE_REGULAR_FILE=  4,
	WASI_FILETYPE_SOCKET_DGRAM : WASI_FILETYPE_SOCKET_DGRAM=  5,
	WASI_FILETYPE_SOCKET_STREAM : WASI_FILETYPE_SOCKET_STREAM=  6,
	WASI_FILETYPE_SYMBOLIC_LINK : WASI_FILETYPE_SYMBOLIC_LINK=  7,

	//////////////////////////////////////////////////////////////
	// WASABI variables
	WASASBI_SERVER_ROOT_PATH : WASASBI_SERVER_ROOT_PATH = null,
	fs_Path2Data : fs_Path2Data = new Map(),
	
	// env : { [key: string]: string | undefined };
	env : env = {},
	fds : fds = [
		{
			vpath:"/dev/stdin",	offset:BigInt(0), data:null,
		},
		{
			vpath:"/dev/stdout", offset:BigInt(0), data:null,
		},
		{
			vpath:"/dev/stderr", offset:BigInt(0), data:null,
		},
		{
			vpath:".", offset:BigInt(0), data:null,
		},
	],
	moduleWasm : moduleWasm = null,
	moduleMemoryView : moduleMemoryView = null,
	
	//////////////////////////////////////////////////////////////
	// WASABI variables
	setModule : setModule = function (module) {
		moduleWasm = module;
		moduleMemoryView = new DataView(getModuleInstanceExports().memory.buffer);
	},
	start : start = function () {
		if(getModuleInstanceExports()._initialize)
			getModuleInstanceExports()._initialize();
		else if(getModuleInstanceExports()._start)
			getModuleInstanceExports()._start();
		else
			throw Error('no _start/_initialize entry point');

		//TODO: Need a specific state 
		if(getModuleInstanceExports().asyncify_stop_unwind) {
			if(sleeping==true) {
				wasabi_log('2 asyncify_stop_unwind');
				getModuleInstanceExports().asyncify_stop_unwind();
			}
		} else {
			wasabi_log('wasm module not asyncified');
		}
	},
	getModuleInstanceExports : getModuleInstanceExports =function () {
		return moduleWasm.instance.exports;
	},
	// Memory
	updatedMemoryView : updatedMemoryView = function() {
		moduleMemoryView = new DataView(getModuleInstanceExports().memory.buffer);
		return moduleMemoryView;
	},
	memoryView : memoryView = function() {
		return moduleMemoryView;
	},
	malloc : malloc = function (size) {
		let ptr = getModuleInstanceExports().malloc(size);
		moduleMemoryView = new DataView(getModuleInstanceExports().memory.buffer);
		return ptr;
	},
	free : free = function (ptr) {
		getModuleInstanceExports().free(ptr);
		moduleMemoryView = new DataView(getModuleInstanceExports().memory.buffer);
	},
	convertJSStr2WAsm : convertJSStr2WAsm =function(js_str)
	{
		let uint8array = new TextEncoder("utf-8").encode(js_str);
		let size = uint8array.length;
		
		let wasm_str = malloc(size+1);
		for (let i = 0; i < size; i++) {
			memoryView().setUint8(wasm_str+i, uint8array[i], true);
		}
		memoryView().setUint8(wasm_str+size, 0, true);
		
		return wasm_str;
	},
	convertWAsmStr2JSStr : convertWAsmStr2JSStr = function (str_ptr)
	{
		let str = "";
		let memory = updatedMemoryView();
		for (let i = str_ptr; memory.getUint8(i) != 0; ++i)
			str += String.fromCharCode(memory.getUint8(i));
		return str;
	},
	
	//////////////////////////////////////////////////////////////
	// WASABI API
	//*************************************************************
	// wasabi specific
	wasabi_getServeFSPath : wasabi_getServeFSPath = function(vpath) {
		let path = null;
		if(WASASBI_SERVER_ROOT_PATH != null && WASASBI_SERVER_ROOT_PATH != "" )
			path = WASASBI_SERVER_ROOT_PATH + vpath;
		else
			path = vpath;

		return path;
	},
	wasabi_initFS : async function(rootFS, paths) {
		WASASBI_SERVER_ROOT_PATH = rootFS;
		for (let i=0; i<paths.length; i++)  {
			vpath = paths[i];
			const response = await fetch(wasabi_getServeFSPath(vpath));
			if (response.status >= 400 && response.status < 600) {
				response.text().then(function (text) {
					throw new Error("Bad response from server response.status='"+response.status+"' " + text + "'");
				  });
			  }
			arrayBuffer = await response.arrayBuffer();
			let uint8View = new Uint8Array(arrayBuffer);
			
			if(vpath.substring(0, 1) == "/")
				vpath = vpath.substring(1);
			fs_Path2Data.set(vpath, uint8View);
			wasabi_log("WASI FileSystem: [" +WASASBI_SERVER_ROOT_PATH + "," + vpath + "] loaded from '" + path + "' (" + uint8View.length + " bytes)");
		}
	},
	wasabi_getFileEntry : wasabi_getFileEntry = function(fd) {
		const entry = fds[fd];
		if (!entry) {
			error("Invalid fd="+fd);
			return null;
		} else if (!entry.vpath) {
			error("No vpath for fd="+fd);
			return null;
		}
		
		if(!entry.data) {
			entry.data = fs_Path2Data.get(entry.vpath);
			if(!entry.data)
			{
				error("Please register your file '" + entry.vpath + "' by calling WASI_API::wasabi_initFS()");
				return null;
			}
			entry.offset = BigInt(0);
		}
		return entry;
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
		console.log(msg);
		if(typeof document !== 'undefined') {
			msg += "\n";
			document.getElementById('log').innerHTML += msg.replace(/\n( *)/g, function (match, p1) {
				return '<br>' + '&nbsp;'.repeat(p1.length);
			});
		}
	},
	wasabi_error : wasabi_error = function(msg) {
		console.error(msg);
		if(typeof document !== 'undefined') {
			msg += "\n";
			document.getElementById('log').innerHTML += msg.replace(/\n( *)/g, function (match, p1) {
				return '<br>' + '&nbsp;'.repeat(p1.length);
			});
		}
	},

	// Clock and time
	clock_res_realtime : clock_res_realtime = function() {
		return BigInt(1e6);
	},

	clock_res_monotonic : clock_res_monotonic = function() {
		return BigInt(1e3);
	},
	clock_res_thread : clock_res_thread = function() {
		return clock_res_monotonic();
	},
	clock_res_process : clock_res_process = function() {
		return clock_res_monotonic();
	},
	clock_time_realtime : clock_time_realtime = function() {
		return BigInt(Date.now()) * BigInt(1e6);
	},

	clock_time_monotonic : clock_time_monotonic = function() {
		const t = performance.now();
		const s = Math.trunc(t);
		const ms = Math.floor((t - s) * 1e3);

		return (BigInt(s) * BigInt(1e9)) + (BigInt(ms) * BigInt(1e6));
	},
	clock_time_process : clock_time_process = function() {
		return clock_time_monotonic();
	},
	clock_time_thread : clock_time_process = function() {
		return clock_time_monotonic();
	},


	//////////////////////////////////////////////////////////////
	// WASI API
	//*************************************************************
	// process
	proc_exit: function(rval) {
		log(Array.prototype.slice.call(arguments));
		error("function proce_exit not yet implemented");
		return WASI_ENOSYS;
	},
	
	//*************************************************************
	// path
	path_open: function(dirfd, dirflags, path_ptr, path_len, oflags, fs_rights_base, fs_rights_inheriting, fs_flags, fd) {
		log(Array.prototype.slice.call(arguments));
		let vpath  = convertWAsmStr2JSStr(path_ptr);
		log("vpath:'" + vpath +"'");
		
		const entry = fds[dirfd];
		if (!entry) {
			error("Invalid dirfd=" + dirfd);
			return WASI_EBADF;
		} else if (!entry.vpath) {
			error("No vpath for dirfd="+dirfd);
			return WASI_EINVAL;
		}

		// Verify that the file is already register during wasabi_initFS
		if(!fs_Path2Data.get(vpath)) {
			if(vpath.substring(0, 1) == ".")
				vpath = vpath.substring(1);
			if(vpath.substring(0, 1) == "/")
				vpath = vpath.substring(1);

			if(!fs_Path2Data.get(vpath))
			{
				error("Please register your file '" + vpath + "' by calling WASI_API::wasabi_initFS()");
				return WASI_EBADF;
			}
			entry.offset = BigInt(0);
		}

		let offset = BigInt(0);
		const opened_fd = fds.push({vpath, offset}) - 1;
		updatedMemoryView().setUint32(fd, opened_fd, true);
			
		return WASI_ESUCCESS;
	},
	path_filestat_get: function(fd, flags, path_ptr, path_len, buf) {
		log(Array.prototype.slice.call(arguments));
		log("vpath:'" + convertWAsmStr2JSStr(path_ptr) +"'");
		error("function path_filestat_get not yet implemented");
		return WASI_ENOSYS;
	},
	path_unlink_file: function(fd, path_ptr, path_len) {
		log(Array.prototype.slice.call(arguments));
		log("vpath:'" + convertWAsmStr2JSStr(path_ptr) +"'");
		error("function path_unlink_file not yet implemented");
		return WASI_ENOSYS;
	},
	
	//*************************************************************
	// async stuff
	poll_oneoff: function(in_, out, nsubscriptions, nevents) {
		log(Array.prototype.slice.call(arguments));
		error("function poll_oneoff not yet implemented");
		return WASI_ENOSYS;
	},
	
	
	//*************************************************************
	// file descriptor
	fd_sync: function(fd) {
		log(Array.prototype.slice.call(arguments));
		error("function ifd_sync not yet implemented");
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
		
		updatedMemoryView().setBigUint64(newoffset_ptr, BigInt(entry.offset), true);

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
		

		let written = 0;
		let bufferBytes = [];                   

		let memory = updatedMemoryView();
		function getiovs(iovs_ptr, iovs_len) {
			// iovs_ptr* -> [iov, iov, ...]
			// __wasi_ciovec_t {
			//   void* buf,
			//   size_t buf_len,
			// }
			let buffers = Array.from({ length: iovs_len }, function (_, i) {
				   let ptr = iovs_ptr + i * 8;
				   let buf = memory.getUint32(ptr, true);
				   let bufLen = memory.getUint32(ptr + 4, true);

				   return new Uint8Array(this.getModuleInstanceExports().memory.buffer, buf, bufLen);
				});

			return buffers;
		}
		let buffers = getiovs(iovs_ptr, iovs_len);
		function writev(iov) {
			let b = 0;
			for (; b < iov.byteLength; b++) {
			   bufferBytes.push(iov[b]);
			}
			written += b;
		}

		buffers.forEach(writev);

		if (fd === WASI_STDOUT_FILENO) wasabi_log(String.fromCharCode.apply(null, bufferBytes));    
		else if (fd === WASI_STDERR_FILENO) wasabi_error(String.fromCharCode.apply(null, bufferBytes));    
		else {
			error("function fd_write Not Yet Implemented");
			return WASI_ENOSYS;
		}

		memory.setUint32(nwritten_ptr, written, true);

		return WASI_ESUCCESS;
	},
	fd_read: function(fd, iovs_ptr, iovs_len, nread_ptr) {
		log(Array.prototype.slice.call(arguments));
		const entry = wasabi_getFileEntry(fd);
		if (!entry) {
			return WASI_EBADF;
		}

		if(getModuleInstanceExports().asyncify_start_unwind)
		{
			if(!sleeping) {
				if(DATA_ADDR  == null)
				{
					DATA_ADDR = malloc(1024*100);
				}
				memoryView().setUint32(DATA_ADDR, DATA_ADDR+8, true);
				memoryView().setUint32(DATA_ADDR+4, 1024*100, true);
				try {
					wasabi_log('1 asyncify_start_unwind');
					getModuleInstanceExports().asyncify_start_unwind(DATA_ADDR);
					sleeping = true;
					wasabi_log('timeout start');
					setTimeout(function() {
						wasabi_log('timeout ended');
						wasabi_log('3 asyncify_start_rewind');
						this.getModuleInstanceExports().asyncify_start_rewind(DATA_ADDR);
						// The code is now ready to rewind; to start the process, enter the
						// first function that should be on the call stack.
						start();
					}, 5000 /*ms*/);
					return;
				}catch(e) {
					error(e);
				}
			} else {
				// We are called as part of a resume/rewind. Stop sleeping.
				wasabi_log('resume');
				wasabi_log('4 asyncify_stop_rewind');
				getModuleInstanceExports().asyncify_stop_rewind();
				sleeping = false;
			}
		}
		else
		{
			wasabi_log('wasm module not asyncified');
		}

		let memory = updatedMemoryView();
		let nread = 0;
		for (let i = 0; i < iovs_len; i++) {
			const data_ptr = memory.getUint32(iovs_ptr, true);
			iovs_ptr += 4;

			const data_len = memory.getUint32(iovs_ptr, true);
			iovs_ptr += 4;

			const data = new Uint8Array(getModuleInstanceExports().memory.buffer, data_ptr, data_len);
			for(let j =0; j < data_len && entry.offset < entry.data.length ; j++) {
				data[j] = entry.data[entry.offset++];
				nread++;
			}
		}
		memory.setUint32(nread_ptr, nread, true);

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
		const entry = wasabi_getFileEntry(fd);
		if (!entry) {
			return WASI_EBADF;
		}

		// Device ID of device containing the file.
		this.updatedMemoryView().setUint32(buf_ptr , 0, true);
		this.memoryView().setUint32(buf_ptr + 4, 0, true);

		// File serial number.
		this.memoryView().setUint32(buf_ptr + 8, 0, true);
		this.memoryView().setUint32(buf_ptr + 12, 0, true);

		// File type.
		this.memoryView().setUint8(buf_ptr + 16, WASI_FILETYPE_REGULAR_FILE, true);
		this.memoryView().setUint32(buf_ptr + 20, 0, true);

		// Number of hard links to the file.
		this.memoryView().setUint32(buf_ptr + 24, 0 , true);
		this.memoryView().setUint32(buf_ptr + 28, 0 , true);
		
		// For regular files, the file size in bytes. For symbolic links, the length in bytes of the pathname contained in the symbolic
		this.memoryView().setUint32(buf_ptr + 32, entry.data.length, true);
		this.memoryView().setUint32(buf_ptr + 36, 0, true);

		// Last data access timestamp.
		this.memoryView().setUint32(buf_ptr + 40, 0, true);
		this.memoryView().setUint32(buf_ptr + 44, 0, true);

		// Last data modification timestamp.
		this.memoryView().setUint32(buf_ptr + 48, 0, true);
		this.memoryView().setUint32(buf_ptr + 52, 0, true);

		// Last file status change timestamp.
		this.memoryView().setUint32(buf_ptr + 56, 0, true);
		this.memoryView().setUint32(buf_ptr + 60, 0, true);

		return WASI_ESUCCESS;
	},
	fd_fdstat_set_flags: function(fd, flags) {
		log(Array.prototype.slice.call(arguments));
		error("function fd_fdstat_set_flags not yet implemented");
	},
	fd_fdstat_get: function(fd, stat_ptr) {
		log(Array.prototype.slice.call(arguments));

		updatedMemoryView().setUint8(stat_ptr, fd);
		memoryView().setUint16(stat_ptr + 2, 0, true);
		memoryView().setUint16(stat_ptr + 4, 0, true);

		memoryView().setUint32(stat_ptr + 8, 0, true);
		memoryView().setUint32(stat_ptr + 12, 0, true);
		memoryView().setUint32(stat_ptr + 16, 0, true);
		memoryView().setUint32(stat_ptr + 20, 0, true);
/*
		function setBigUint64(byteOffset, value, littleEndian) {
			let lowWord = value;
			let highWord = 0;

			memoryView().setUint32(littleEndian ? 0 : 4, lowWord, littleEndian);
			memoryView().setUint32(littleEndian ? 4 : 0, highWord, littleEndian);
		}
*/
	//	setBigUint64(stat_ptr + 8, 0, true);
	//	setBigUint64(stat_ptr + 8 + 8, 0, true);

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

		let memory = updatedMemoryView();
		memory.setUint8(buf_out, WASI_PREOPENTYPE_DIR);
		memory.setUint32(buf_out + 4, new TextEncoder().encode(entry.vpath).byteLength, true);
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
		log("vpath:'" + convertWAsmStr2JSStr(path_ptr) +"'");

		const data = new Uint8Array(this.getModuleInstanceExports().memory.buffer, path_ptr, path_len);
		data.set(new TextEncoder().encode(entry.vpath));

		return WASI_ESUCCESS;
	},
	
	
	//*************************************************************
	// var env
	environ_sizes_get: function(environ_size, environ_buf_size) {
		log(Array.prototype.slice.call(arguments));
		const entries = Object.entries(env);
		const text = new TextEncoder();

		let memory = updatedMemoryView();
		memory.setUint32(environ_size, entries.length, true);
		memory.setUint32(environ_buf_size, entries.reduce(function(acc, [key, value]) {
			return acc + text.encode(`${key}=${value}\0`).length;
		}, 0), true);

		return WASI_ESUCCESS;
	},
	environ_get: function(environ_ptr, environ_buf_ptr) {
		log(Array.prototype.slice.call(arguments));
		const entries = Object.entries(env);
		const text = new TextEncoder();
		const heap = new Uint8Array(this.memory.buffer);

		let memory = updatedMemoryView();
		for (let [key, value] of entries) {
			memory.setUint32(environ_ptr, environ_buf_ptr, true);
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
		let memory = this.updatedMemoryView();
		switch (clock_id) {
			case WASI_CLOCKID_REALTIME:
				memory.setBigUint64(resolution_out, clock_res_realtime(), true);
				break;

			case WASI_CLOCKID_MONOTONIC:
				memory.setBigUint64(resolution_out, clock_res_monotonic(), true);
				break;

			case WASI_CLOCKID_PROCESS_CPUTIME_ID:
				memory.setBigUint64(resolution_out, clock_res_process(), true);
				break;

			case WASI_CLOCKID_THREAD_CPUTIME_ID:
				memory.setBigUint64(resolution_out, clock_res_thread(), true);
				break;

			default:
				return WASI_INVAL;

		}

		return WASI_ESUCCESS;
	},
	clock_time_get: function(clock_id, precision, time) {
		let memory = this.updatedMemoryView();
		log(Array.prototype.slice.call(arguments));
		switch (clock_id) {
			case WASI_CLOCKID_REALTIME:
				memory.setBigUint64(time, clock_time_realtime(), true);
				break;

			case WASI_CLOCKID_MONOTONIC:
				memory.setBigUint64(time, clock_time_monotonic(), true);
				break;

			case WASI_CLOCKID_PROCESS_CPUTIME_ID:
				memory.setBigUint64(time, clock_time_process(), true);
				break;

			case WASI_CLOCKID_THREAD_CPUTIME_ID:
				memory.setBigUint64(time, clock_time_thread(), true);
				break;

			default:
				return WASI_INVAL;

		}
		return WASI_ESUCCESS;
	},
};
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
