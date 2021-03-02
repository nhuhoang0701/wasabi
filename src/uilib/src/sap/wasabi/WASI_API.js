/*!
 * ${copyright}
 */
/*global sap*/
// https://docs.rs/wasi/0.10.2+wasi-snapshot-preview1/wasi/wasi_snapshot_preview1/index.html
sap.ui.define(
  [
    "sap/base/Log",
    "sap/wasabi/thirdparty/lodash"
  ],
  function(Log,_){
    var moduleInstanceExports = null;
    function clock_res_realtime () {
      return window.BigInt(1e6);
    }
    function clock_res_monotonic() {
      return window.BigInt(1e3);
    }
    var clock_res_process = clock_res_monotonic;
    var clock_res_thread = clock_res_monotonic;
    function clock_time_realtime () {
      return window.BigInt(Date.now()) * window.BigInt(1e6);
    }
    function clock_time_monotonic () {
      var t = window.performance.now();
      var s = Math.trunc(t);
      var ms = Math.floor((t - s) * 1e3);
      return (
        window.BigInt(s) * window.BigInt(1e9)) + (window.BigInt(ms) * window.BigInt(1e6));
    }
    var clock_time_process = clock_time_monotonic;
    var clock_time_thread = clock_time_monotonic;
    window.fs_Path2Data = new window.Map();
    var fds = [
      {
        vpath:"/dev/stdin",
        offset:null,
        data:null,
      },
      {
        vpath:"/dev/stdout",
        offset:null,
        data:null,
      },
      {
        vpath:"/dev/stderr",
        offset:null,
        data:null,
      },
      {
        vpath:".",
        offset:null,
        data:null,
      },
    ];
    function convertWAsmStr2JSStr(str_ptr, memory)
    {
      var heap = new window.Uint8Array(memory.buffer);
      var str = "";
      for (var i = str_ptr; heap[i] != 0; ++i)
        str += String.fromCharCode(heap[i]);
      return str;
    }
    function convertJSStr2WAsm(js_str, module)
    {
      var heap = new window.Uint8Array(module.instance.exports.memory.buffer);
      var uint8array = new window.TextEncoder("utf-8").encode(js_str);
      var size = uint8array.length;
      var wasm_str = module.instance.exports.malloc(size+1);
      for (var i = 0; i < size; i++) {
        heap[wasm_str+i] = uint8array[i];
      }
      heap[wasm_str+size] = 0;
      return wasm_str;
    }
    function getModuleMemoryDataView() {
      return new window.DataView(moduleInstanceExports.memory.buffer);
    }
    function setModuleInstance(instance) {
      moduleInstanceExports = instance.exports;
    }
    function getModuleInstance() {
      return moduleInstanceExports;
    }
    var WASI_API = {
      // WASI constants
      WASI_ESUCCESS: WASI_API.WASI_ESUCCESS = 0,
      WASI_EBADF : WASI_API.WASI_EBADF = 8,
      WASI_EINVAL : WASI_API.WASI_EINVAL = 28,
      WASI_ENOSYS : WASI_API.WASI_ENOSYS = 52,
      WASI_STDOUT_FILENO : WASI_API.WASI_STDOUT_FILENO = 1,
      WASI_STDERR_FILENO : WASI_API.WASI_STDERR_FILENO = 2,
      WASI_SEEK_START : WASI_API.WASI_SEEK_START = 0,
      WASI_SEEK_CUR : WASI_API.WASI_SEEK_CUR = 1,
      WASI_SEEK_END : WASI_API.WASI_SEEK_END = 2,
      WASI_PREOPENTYPE_DIR : WASI_API.WASI_PREOPENTYPE_DIR = 0,
      // WASI API
      //*************************************************************
      // wasabi specific
      wasabi_initFS : function(paths) {
        return Promise.all(
          _.map(
            paths,
            function( sPath){
              var response;
              return window.fetch(
                sPath
              ).then(
                function(o){
                  response = o;
                  return response.arrayBuffer();
                }
              ).then(
                function(o){
                  window.arrayBuffer  = o;
                  var uint8View = new window.Uint8Array(window.arrayBuffer);
                  window.fs_Path2Data.set(sPath, uint8View);
                }
              ).catch( function(oError){
                Log.error(oError);
                throw oError;
              });
            }
          )
        );
      },
      wasabi_log : function(msg) {
        Log.info(msg);
      },
      getModuleInstance: getModuleInstance,
      setModuleInstance: setModuleInstance,
      convertJSStr2WAsm: convertJSStr2WAsm,
      wasabi_error : function(msg) {
        Log.error(msg);
      },
      //*************************************************************
      // process
      proc_exit: function() {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        return WASI_API.WASI_ENOSYS;
      },
      //*************************************************************
      // path
      path_open: function(dirfd, dirflags, path_ptr, path_len, oflags, fs_rights_base, fs_rights_inheriting, fs_flags, fd) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        var vpath  = convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory);
        Log.info("vpath:'" + vpath +"'");
        var entry = fds[dirfd];
        if (!entry) {
          Log.error('Invalid dirfd');
          return WASI_API.WASI_EBADF;
        } else if (!entry.vpath) {
          Log.error('No vpath for dirfd='+dirfd);
          return WASI_API.WASI_EINVAL;
        }
        /*
        var text = new window.TextDecoder();
        var data = new Uint8Array(moduleInstanceExports.memory.buffer, path_ptr, path_len);
        var size = fds.length;
        */
        var offset = window.BigInt(0);
        var opened_fd = fds.push([vpath, offset]) - 1;
        var view = new window.DataView(moduleInstanceExports.memory.buffer);
        view.setUint32(fd, opened_fd, true);
        return WASI_API.WASI_ESUCCESS;
      },
      path_filestat_get: function(fd, flags, path_ptr) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        Log.info("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");
        return WASI_API.WASI_ENOSYS;
      },
      path_unlink_file: function(fd, path_ptr) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        Log.info("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");
        return WASI_API.WASI_ENOSYS;
      },
      poll_oneoff: function() {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        return WASI_API.WASI_ENOSYS;
      },
      //*************************************************************
      // file descriptor
      fd_sync: function() {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        return WASI_API.WASI_ENOSYS;
      },
      fd_seek: function(fd, offset, whence, newoffset_ptr) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        var entry = fds[fd];
        if (!entry) {
          Log.error("Invalid fd");
          return WASI_API.WASI_EBADF;
        }
        if(whence == WASI_API.WASI_SEEK_START)
          entry.offset = offset;
        else if(whence == WASI_API.WASI_SEEK_CUR) {
          entry.offset += offset;
        } else if(whence == WASI_API.WASI_SEEK_END) {
          entry.offset += offset;
        } else
          Log.error("NYI");
        if(entry.offset < 0)
          entry.offset = 0;
        var view = new window.DataView(moduleInstanceExports.memory.buffer);
        view.setBigUint64(newoffset_ptr, window.BigInt(entry.offset), true);
        return WASI_API.WASI_ESUCCESS;
      },
      fd_write: function(fd, iovs_ptr, iovs_len, nwritten_ptr) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        var entry = fds[fd];
        if (!entry) {
          Log.error('Invalid fd');
          return WASI_API.WASI_EBADF;
        } else if (!entry.vpath) {
          Log.error("No vpath for fd="+fd);
          return WASI_API.WASI_EINVAL;
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
            return new window.Uint8Array(moduleInstanceExports.memory.buffer, buf, bufLen);
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
        if (fd === WASI_API.WASI_STDOUT_FILENO){
          WASI_API.wasabi_log(String.fromCharCode.apply(null, bufferBytes));
        }
        else if (fd === WASI_API.WASI_STDERR_FILENO){
          WASI_API.wasabi_error(String.fromCharCode.apply(null, bufferBytes));
        } else{
          Log.error("fd_write NYI");
        }
        view.setUint32(nwritten_ptr, written, !0);
        return WASI_API.WASI_ESUCCESS;
      },
      fd_read: function(fd, iovs_ptr, iovs_len, nread_ptr) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        var entry = fds[fd];
        if (!entry) {
          console.error('Invalid fd');
          return WASI_API.WASI_EBADF;
        } else if (!entry.vpath) {
          Log.error('No vpath for fd='+fd);
          return WASI_API.WASI_EINVAL;
        }
        if(!entry.data) {
          entry.data = WASI_API.fs_Path2Data.get(entry.vpath);
          entry.offset = 0;
        }
        var view = new window.DataView(moduleInstanceExports.memory.buffer);
        var nread = 0;
        var i;
        for ( i = 0; i < iovs_len; i++) {
          var data_ptr = view.getUint32(iovs_ptr, true);
          iovs_ptr += 4;
          var data_len = view.getUint32(iovs_ptr, true);
          iovs_ptr += 4;
          var data = new window.Uint8Array(moduleInstanceExports.memory.buffer, data_ptr, data_len);
          var j;
          for( j =0; j < data_len && entry.offset < entry.data.length ; j++) {
            data[j] = entry.data[entry.offset++];
            nread++;
          }
        }
        Log.info("char readed:" +nread);
        view.setUint32(nread_ptr, nread, true);
        return WASI_API.WASI_ESUCCESS;
      },
      fd_close: function(fd) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        var entry = fds[fd];
        if (!entry) {
          console.error('Invalid fd');
          return WASI_API.WASI_EBADF;
        }
        fds.splice(fd);
        return WASI_API.WASI_ESUCCESS;
      },
      fd_filestat_get: function() {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        return WASI_API.WASI_ENOSYS;
      },
      fd_fdstat_set_flags: function() {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
      },
      fd_fdstat_get: function(fd, stat_ptr) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
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
        return WASI_API.WASI_ESUCCESS;
      },
      fd_prestat_get: function(fd, buf_out) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        var entry = fds[fd];
        if (!entry) {
          Log.error("Invalid fd");
          return WASI_API.WASI_EBADF;
        } else if (!entry.vpath) {
          Log.error('No vpath for fd='+fd);
          return WASI_API.WASI_EBADF;
        }
        Log.info("vpath:'" + entry.vpath +"'");
        var view = new window.DataView(moduleInstanceExports.memory.buffer);
        view.setUint8(buf_out, WASI_API.WASI_PREOPENTYPE_DIR);
        view.setUint32(buf_out + 4, new window.TextEncoder().encode(entry.vpath).byteLength, true);
        return WASI_API.WASI_ESUCCESS;
      },
      fd_prestat_dir_name: function(fd, path_ptr, path_len) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        var entry = fds[fd];
        if (!entry) {
          return WASI_API.WASI_EBADF;
        }
        if (!entry.vpath) {
          return WASI_API.WASI_EBADF;
        }
        Log.info("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");
        var data = new window.Uint8Array(moduleInstanceExports.memory.buffer, path_ptr, path_len);
        data.set(new window.TextEncoder().encode(entry.vpath));
        return WASI_API.WASI_ESUCCESS;
      },
      //*************************************************************
      // var env
      environ_sizes_get: function(environ_size, environ_buf_size) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        var view = getModuleMemoryDataView();
        view.setUint32(environ_size, 0, !0);
        view.setUint32(environ_buf_size, 0, !0);
        return WASI_API.WASI_ESUCCESS;
      },
      environ_get: function() {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
      },
      //*************************************************************
      // Clock
      clock_res_process: clock_res_process,
      clock_res_thread: clock_res_thread,
      clock_res_realtime: clock_res_realtime,
      clock_time_get: function(clock_id, precision, time) {
        Log.info("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
        var view = getModuleMemoryDataView();
        switch (clock_id) {
        case WASI_API.WASI_CLOCKID_REALTIME1:
          view.setBigUint64(time, clock_time_realtime(), true);
          break;
        case WASI_API.WASI_CLOCKID_MONOTONIC:
          view.setBigUint64(time, clock_time_monotonic(), true);
          break;
        case WASI_API.WASI_CLOCKID_PROCESS_CPUTIME_ID:
          view.setBigUint64(time, clock_time_process(), true);
          break;
        case WASI_API.WASI_CLOCKID_THREAD_CPUTIME_ID:
          view.setBigUint64(time, clock_time_thread(), true);
          break;
        default:
          return WASI_API.WASI_INVAL;
        }
        return WASI_API.WASI_ESUCCESS;
      },
    };
    return WASI_API;
  }
);
