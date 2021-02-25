// Members
moduleInstanceExports = null;

fds = [
		{
			vpath:"/dev/stdin",
		},
		{
			vpath:"/dev/stdout",
		},
		{
			vpath:"/dev/stderr",
		},
		{
			vpath:".",
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
	
	WASI_PREOPENTYPE_DIR : WASI_PREOPENTYPE_DIR = 0,

	// WASI API
	proc_exit: function(rval) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		return WASI_ENOSYS;
	},
	path_open: function(dirfd, dirflags, path, path_len, oflags, fs_rights_base, fs_rights_inheriting, fs_flags, fd) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		console.log("vpath:'" + convertWAsmStr2JSStr(path, moduleInstanceExports.memory) +"'");
		return WASI_ENOSYS;
	},
	path_filestat_get: function(fd, path, path_len, buf) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		console.log("vpath:'" + convertWAsmStr2JSStr(path, moduleInstanceExports.memory) +"'");
		return WASI_ENOSYS;
	},
	path_unlink_file: function(fd, path, path_len, flags) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		console.log("vpath:'" + convertWAsmStr2JSStr(path, moduleInstanceExports.memory) +"'");
		return WASI_ENOSYS;
	},
	poll_oneoff: function(in_, out, nsubscriptions, nevents) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		return WASI_ENOSYS;
	},
	
	fd_sync: function(fd) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		return WASI_ENOSYS;
	},
	fd_seek: function(fd, offset, whence, newoffset) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		return WASI_ENOSYS;
	},
	fd_write: function(fd, iovs, iovs_len, nwritten) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		const entry = fds[fd];
		if (!entry) {
			console.error('Invalid fd');
			return WASI_EBADF;
		} else if (!entry.vpath) {
			console.error('No vpath for fd='+fd);
			return ERRNO_INVAL;
		}
		
		var view = getModuleMemoryDataView();

		var written = 0;
		var bufferBytes = [];                   

		function getiovs(iovs, iovs_len) {
			// iovs* -> [iov, iov, ...]
			// __wasi_ciovec_t {
			//   void* buf,
			//   size_t buf_len,
			// }
			var buffers = Array.from({ length: iovs_len }, function (_, i) {
				   var ptr = iovs + i * 8;
				   var buf = view.getUint32(ptr, !0);
				   var bufLen = view.getUint32(ptr + 4, !0);

				   return new Uint8Array(moduleInstanceExports.memory.buffer, buf, bufLen);
				});

			return buffers;
		}

		var buffers = getiovs(iovs, iovs_len);
		function writev(iov) {
			for (var b = 0; b < iov.byteLength; b++) {
			   bufferBytes.push(iov[b]);
			}
			written += b;
		}

		buffers.forEach(writev);

		if (fd === WASI_STDOUT_FILENO) console.log(String.fromCharCode.apply(null, bufferBytes));    
		else if (fd === WASI_STDERR_FILENO) console.error(String.fromCharCode.apply(null, bufferBytes));                            

		view.setUint32(nwritten, written, !0);

		return WASI_ESUCCESS;
	},
	fd_read: function(fd, iovs, iovs_len, nread) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
	},
	fd_close: function(fd) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		return WASI_ENOSYS;
	},
	fd_filestat_get: function(fd, buf) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		return WASI_ENOSYS;
	},
	fd_fdstat_set_flags: function(fd, flags) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
	},
	fd_fdstat_get: function(fd, buf) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		var view = getModuleMemoryDataView();

		view.setUint8(buf, fd);
		view.setUint16(buf + 2, 0, !0);
		view.setUint16(buf + 4, 0, !0);

		function setBigUint64(byteOffset, value, littleEndian) {
			var lowWord = value;
			var highWord = 0;

			view.setUint32(littleEndian ? 0 : 4, lowWord, littleEndian);
			view.setUint32(littleEndian ? 4 : 0, highWord, littleEndian);
		}

		setBigUint64(buf + 8, 0, !0);
		setBigUint64(buf + 8 + 8, 0, !0);

		return WASI_ESUCCESS;
	},
	fd_prestat_get: function(fd, buf_out) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		const entry = fds[fd];
		if (!entry) {
			console.error('Invalid fd');
			return WASI_EBADF;
		} else if (!entry.vpath) {
			console.error('No vpath for fd='+fd);
			return WASI_EBADF;
		}
		console.log("vpath:'" + entry.vpath +"'");

		const view = new DataView(moduleInstanceExports.memory.buffer);
		view.setUint8(buf_out, WASI_PREOPENTYPE_DIR);
		view.setUint32(buf_out + 4, new TextEncoder().encode(entry.vpath).byteLength, true);
		return WASI_ESUCCESS;
	},
	fd_prestat_dir_name: function(fd, path, path_len) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		const entry = fds[fd];
		if (!entry) {
			return WASI_EBADF;
		}
		if (!entry.vpath) {
			return WASI_EBADF;
		}
		console.log("vpath:'" + convertWAsmStr2JSStr(path, moduleInstanceExports.memory) +"'");

		const data = new Uint8Array(moduleInstanceExports.memory.buffer, path, path_len);
		data.set(new TextEncoder().encode(entry.vpath));

		return WASI_ESUCCESS;
	},
	environ_sizes_get: function(environ_size, environ_buf_size) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		var view = getModuleMemoryDataView();

		view.setUint32(environ_size, 0, !0);
		view.setUint32(environ_buf_size, 0, !0);

		return WASI_ESUCCESS;
	},
	environ_get: function(environ, environ_buf) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
	},
	clock_time_get: function(clock_id, precision, time) {
		console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
		return WASI_EINVAL;
	},
};
