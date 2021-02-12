
function _handleFiles(fileInput) {
	if (fileInput.length == 0) {
		console.error("Missing file");
		return;
	}
	
	var file = fileInput[0];
	if(file.type !== "application/wasm" ) {
		console.error("File type is not 'application/wasm' but was '" + file.type + "'");
		return;
	}
	let response = new Response(file);
	start(response);
};

function start(response) {
	var moduleInstanceExports = null;
	function getModuleMemoryDataView() {
		return new DataView(moduleInstanceExports.memory.buffer);
	}
	function setModuleInstance(instance) {
		moduleInstanceExports = instance.exports;
	}
	
	var WASI_ESUCCESS = 0;
	var WASI_EBADF = 8;
	var WASI_EINVAL = 28;
	var WASI_ENOSYS = 52;
	
	var WASI_STDOUT_FILENO = 1;
	var WASI_API = {
		proc_exit: function(rval) {
			console.error("NYI:" + arguments.callee.name);
			return WASI_ENOSYS;
		},
		path_open: function(dirfd, dirflags, path, path_len, oflags, fs_rights_base, fs_rights_inheriting, fs_flags, fd) {
			throw new Error("NYI:" + arguments.callee.name);
		},
		path_filestat_get: function(fd, path, path_len, buf) {
			console.error("NYI:" + arguments.callee.name);
			return WASI_ENOSYS;
		},
		path_unlink_file: function(fd, path, path_len, flags) {
			console.error("NYI:" + arguments.callee.name);
			return WASI_ENOSYS;
		},
		poll_oneoff: function(in_, out, nsubscriptions, nevents) {
			console.error("NYI:" + arguments.callee.name);
			return WASI_ENOSYS;
		},
		
		fd_sync: function(fd) {
			console.error("NYI:" + arguments.callee.name);
			return WASI_ENOSYS;
		},
		fd_seek: function(fd, offset, whence, newoffset) {
			throw new Error("NYI:" + arguments.callee.name);
		},
		fd_write: function(fd, iovs, iovs_len, nwritten) {
			console.log("WASI:" + arguments.callee.name);
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

			view.setUint32(nwritten, written, !0);

			return WASI_ESUCCESS;
		},
		fd_read: function(fd, iovs, iovs_len, nread) {
			console.error("NYI:" + arguments.callee.name);
		},
		fd_close: function(fd) {
			console.error("NYI:" + arguments.callee.name);
			return WASI_ENOSYS;
		},
		fd_filestat_get: function(fd, buf) {
			console.error("NYI:" + arguments.callee.name);
			return WASI_ENOSYS;
		},
		fd_fdstat_set_flags: function(fd, flags) {
			console.error("NYI:" + arguments.callee.name);
		},
		fd_fdstat_get: function(fd, buf) {
			console.log("WASI:" + arguments.callee.name);
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
		fd_prestat_get: function(fd, buf) {
			console.error("NYI:" + arguments.callee.name);
			return WASI_EBADF;
		},
		fd_prestat_dir_name: function(fd, path, path_len) {
			console.error("NYI:" + arguments.callee.name);
			return WASI_EINVAL;
		},
		environ_sizes_get: function(environ_size, environ_buf_size) {
			console.error("NYI:" + arguments.callee.name);
			var view = getModuleMemoryDataView();

			view.setUint32(environ_size, 0, !0);
			view.setUint32(environ_buf_size, 0, !0);

			return WASI_ESUCCESS;
		},
		environ_get: function(environ, environ_buf) {
			console.error("NYI:" + arguments.callee.name);
		},
		clock_time_get: function(clock_id, precision, time) {
			console.error("NYI:" + arguments.callee.name);
			return WASI_EINVAL;
		},
	};
	const importObject =
	{
		wasi_snapshot_preview1: WASI_API,
		env: {},
		js : {mem: new WebAssembly.Memory({initial: 2,maximum: 2})}
	};

	WebAssembly.instantiateStreaming(response, importObject).then(module =>
	{
		setModuleInstance(module.instance);
		try {module.instance.exports._start();}
		catch (e)
		{
			console.log("Exception during execution:" + e);
		}
	});
};
