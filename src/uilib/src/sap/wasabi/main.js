/*!
 * ${copyright}
 */
/*global sap, main*/
// https://docs.rs/wasi/0.10.2+wasi-snapshot-preview1/wasi/wasi_snapshot_preview1/index.html
sap.ui.define(
  [
    "sap/base/Log",
    "sap/wasabi/BridgedWorker"
  ],
  function(Log,BuildBridgedWorker){
    var n = 0;
    var oPromHash = {};
    var WriteLog = function( s,b ){
      if(b){
        Log.error(s);
      }else{
        Log.info(s);
      }
    };
    var ResolvePromise = function (nId,mValue) {
      var oResolver = oPromHash[nId];
      delete oPromHash[nId];

      if( mValue instanceof Error){
        oResolver.rej(mValue);
      }else{
        oResolver.res(mValue);
      }
    };
    var Main = function(){
      var that = this;
      var oWorker = BuildBridgedWorker(
        function (){
          var oWasabi;
          var WASI_API = (function(){
            var moduleInstanceExports = null;
            function clock_res_realtime () {
              return BigInt(1e6);
            }
            function clock_res_monotonic() {
              return BigInt(1e3);
            }
            var clock_res_process = clock_res_monotonic;
            var clock_res_thread = clock_res_monotonic;
            function clock_time_realtime () {
              return BigInt(Date.now()) * BigInt(1e6);
            }
            function clock_time_monotonic () {
              var t = performance.now();
              var s = Math.trunc(t);
              var ms = Math.floor((t - s) * 1e3);
              return (
                BigInt(s) * BigInt(1e9)) + (BigInt(ms) * BigInt(1e6));
            }
            var clock_time_process = clock_time_monotonic;
            var clock_time_thread = clock_time_monotonic;
            fs_Path2Data = new Map();
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
              var heap = new Uint8Array(memory.buffer);
              var str = "";
              for (var i = str_ptr; heap[i] != 0; ++i)
                str += String.fromCharCode(heap[i]);
              return str;
            }
            function convertJSStr2WAsm(js_str, moduleInstance)
            {
              var heap = new Uint8Array(moduleInstance.memory);
              var uint8array = new TextEncoder("utf-8").encode(js_str);
              var size = uint8array.length;
              var wasm_str = moduleInstance.malloc(size+1);
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
              WASI_ESUCCESS:  0,
              WASI_EBADF :  8,
              WASI_EINVAL :  28,
              WASI_ENOSYS : 52,
              WASI_STDOUT_FILENO :  1,
              WASI_STDERR_FILENO : 2,
              WASI_SEEK_START : 0,
              WASI_SEEK_CUR : 1,
              WASI_SEEK_END : 2,
              WASI_PREOPENTYPE_DIR : 0,
              // WASI API
              //*************************************************************
              // wasabi specific
              wasabi_initFS : function(paths) {
                return Promise.all(
                  paths.map(
                    function( sPath){
                      var response;
                      return fetch(
                        sPath
                      ).then(
                        function(o){
                          response = o;
                          return response.arrayBuffer();
                        }
                      ).then(
                        function(o){
                          arrayBuffer  = o;
                          var uint8View = new Uint8Array(arrayBuffer);
                          fs_Path2Data.set(sPath, uint8View);
                        }
                      ).catch( function(oError){
                        throw oError;
                      });
                    }
                  )
                );
              },
              wasabi_log : function(msg) {
                main.WriteLog(msg);
              },
              getModuleInstance: getModuleInstance,
              setModuleInstance: setModuleInstance,
              convertJSStr2WAsm: convertJSStr2WAsm,
              convertWAsmStr2JSStr: convertWAsmStr2JSStr,
              wasabi_error : function(msg) {
                main.WriteLog(msg,true);
              },
              //*************************************************************
              // process
              proc_exit: function() {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                return WASI_API.WASI_ENOSYS;
              },
              //*************************************************************
              // path
              path_open: function(dirfd, dirflags, path_ptr, path_len, oflags, fs_rights_base, fs_rights_inheriting, fs_flags, fd) {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                var vpath  = convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory);
                main.WriteLog("vpath:'" + vpath +"'");
                var entry = fds[dirfd];
                if (!entry) {
                  main.WriteLog("Invalid dirfd",true);
                  return WASI_API.WASI_EBADF;
                } else if (!entry.vpath) {
                  main.WriteLog('No vpath for dirfd='+dirfd,true);
                  return WASI_API.WASI_EINVAL;
                }
                /*
                  var text = new TextDecoder();
                  var data = new Uint8Array(moduleInstanceExports.memory.buffer, path_ptr, path_len);
                  var size = fds.length;
                */
                var offset = BigInt(0);
                var opened_fd = fds.push([vpath, offset]) - 1;
                var view = new DataView(moduleInstanceExports.memory.buffer);
                view.setUint32(fd, opened_fd, true);
                return WASI_API.WASI_ESUCCESS;
              },
              path_filestat_get: function(fd, flags, path_ptr) {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                main.WriteLog("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");
                return WASI_API.WASI_ENOSYS;
              },
              path_unlink_file: function(fd, path_ptr) {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                main.WriteLog("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");
                return WASI_API.WASI_ENOSYS;
              },
              poll_oneoff: function() {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                return WASI_API.WASI_ENOSYS;
              },
              //*************************************************************
              // file descriptor
              fd_sync: function() {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                return WASI_API.WASI_ENOSYS;
              },
              fd_seek: function(fd, offset, whence, newoffset_ptr) {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                var entry = fds[fd];
                if (!entry) {
                  main.WriteLog("Invalid fd");
                  return WASI_API.WASI_EBADF;
                }
                if(whence == WASI_API.WASI_SEEK_START)
                  entry.offset = offset;
                else if(whence == WASI_API.WASI_SEEK_CUR) {
                  entry.offset += offset;
                } else if(whence == WASI_API.WASI_SEEK_END) {
                  entry.offset += offset;
                } else
                  main.WriteLog("NYI",true);
                if(entry.offset < 0)
                  entry.offset = 0;
                var view = new DataView(moduleInstanceExports.memory.buffer);
                view.setBigUint64(newoffset_ptr, BigInt(entry.offset), true);
                return WASI_API.WASI_ESUCCESS;
              },
              fd_write: function(fd, iovs_ptr, iovs_len, nwritten_ptr) {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                var entry = fds[fd];
                if (!entry) {
                  main.WriteLog("Invalid fd",true);
                  return WASI_API.WASI_EBADF;
                } else if (!entry.vpath) {
                  main.WriteLog("No vpath for fd="+fd,true);
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
                if (fd === WASI_API.WASI_STDOUT_FILENO){
                  WASI_API.wasabi_log(String.fromCharCode.apply(null, bufferBytes));
                }
                else if (fd === WASI_API.WASI_STDERR_FILENO){
                  WASI_API.wasabi_error(String.fromCharCode.apply(null, bufferBytes));
                } else{
                  main.WriteLog("fd_write NYI",true);
                }
                view.setUint32(nwritten_ptr, written, !0);
                return WASI_API.WASI_ESUCCESS;
              },
              fd_read: function(fd, iovs_ptr, iovs_len, nread_ptr) {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                var entry = fds[fd];
                if (!entry) {
                  console.error('Invalid fd');
                  return WASI_API.WASI_EBADF;
                } else if (!entry.vpath) {
                  main.WriteLog('No vpath for fd='+fd,true);
                  return WASI_API.WASI_EINVAL;
                }
                if(!entry.data) {
                  entry.data = WASI_API.fs_Path2Data.get(entry.vpath);
                  entry.offset = 0;
                }
                var view = new DataView(moduleInstanceExports.memory.buffer);
                var nread = 0;
                var i;
                for ( i = 0; i < iovs_len; i++) {
                  var data_ptr = view.getUint32(iovs_ptr, true);
                  iovs_ptr += 4;
                  var data_len = view.getUint32(iovs_ptr, true);
                  iovs_ptr += 4;
                  var data = new Uint8Array(moduleInstanceExports.memory.buffer, data_ptr, data_len);
                  var j;
                  for( j =0; j < data_len && entry.offset < entry.data.length ; j++) {
                    data[j] = entry.data[entry.offset++];
                    nread++;
                  }
                }
                main.WriteLog("char readed:" +nread);
                view.setUint32(nread_ptr, nread, true);
                return WASI_API.WASI_ESUCCESS;
              },
              fd_close: function(fd) {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                var entry = fds[fd];
                if (!entry) {
                  console.error('Invalid fd');
                  return WASI_API.WASI_EBADF;
                }
                fds.splice(fd);
                return WASI_API.WASI_ESUCCESS;
              },
              fd_filestat_get: function() {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                return WASI_API.WASI_ENOSYS;
              },
              fd_fdstat_set_flags: function() {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
              },
              fd_fdstat_get: function(fd, stat_ptr) {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
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
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                var entry = fds[fd];
                if (!entry) {
                  main.WriteLog("Invalid fd",true);
                  return WASI_API.WASI_EBADF;
                } else if (!entry.vpath) {
                  main.WriteLog("No vpath for fd="+fd,true);
                  return WASI_API.WASI_EBADF;
                }
                main.WriteLog("vpath:'" + entry.vpath +"'");
                var view = new DataView(moduleInstanceExports.memory.buffer);
                view.setUint8(buf_out, WASI_API.WASI_PREOPENTYPE_DIR);
                view.setUint32(buf_out + 4, new TextEncoder().encode(entry.vpath).byteLength, true);
                return WASI_API.WASI_ESUCCESS;
              },
              fd_prestat_dir_name: function(fd, path_ptr, path_len) {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                var entry = fds[fd];
                if (!entry) {
                  return WASI_API.WASI_EBADF;
                }
                if (!entry.vpath) {
                  return WASI_API.WASI_EBADF;
                }
                main.WriteLog("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");
                var data = new Uint8Array(moduleInstanceExports.memory.buffer, path_ptr, path_len);
                data.set(new TextEncoder().encode(entry.vpath));
                return WASI_API.WASI_ESUCCESS;
              },
              //*************************************************************
              // var env
              environ_sizes_get: function(environ_size, environ_buf_size) {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
                var view = getModuleMemoryDataView();
                view.setUint32(environ_size, 0, !0);
                view.setUint32(environ_buf_size, 0, !0);
                return WASI_API.WASI_ESUCCESS;
              },
              environ_get: function() {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
              },
              //*************************************************************
              // Clock
              clock_res_process: clock_res_process,
              clock_res_thread: clock_res_thread,
              clock_res_realtime: clock_res_realtime,
              clock_time_get: function(clock_id, precision, time) {
                main.WriteLog("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
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
          }());
          var WASI_MODULE =( function(){
            function start(response) {
              var importObject =
                  {
                    wasi_snapshot_preview1: WASI_API,
                    env: {},
                    js : {
                      mem: new WebAssembly.Memory(
                        {
                          initial: 2,
                          maximum: 10
                        }
                      )
                    }
                  };
              return WebAssembly.instantiateStreaming(
                response, importObject
              ).then(
                function(module){
                  WASI_API.setModuleInstance(module.instance);
                  return WASI_API.wasabi_initFS(
                    [
                      "../resources/text.txt",
                      "../sqlite/efashion.db"
                    ]
                  );
                }
              ).then(
                function(module){
                  return module.instance.exports._start();
                }
              ).catch(
                function(e){
                  main.WriteLog("Exception during execution:" + e,true);
                  throw e;
                }
              );
            }
            var WASIModule = {
              handleFiles: function(fileInput) {
                if (fileInput.length == 0) {
                  Log.error("Missing file");
                  return;
                }
                var file = fileInput[0];
                if(file.type !== "application/wasm" ) {
                  Log.error("File type is not 'application/wasm' but was '" + file.type + "'");
                  return;
                }
                var response = new Response(file);
                start(response);
              }
            };
            return WASIModule;
          }());
          function ensureWasabi(){
            if(!oWasabi){
            oWasabi = fetch(
              "http://localhost:8080/resources/sap/wasabi/InA_Interpreter.wasm"
            ).then(
              function(oRes){
                var importObject = {
                  wasi_snapshot_preview1: WASI_API,
                  env: {},
                  js : {
                    mem: new WebAssembly.Memory(
                      {
                        initial: 2
                        ,maximum: 10
                      }
                    )
                  }
                };
                return WebAssembly.instantiateStreaming(
                  oRes, importObject
                );
              }
            ).then(
              function(module){
                WASI_API.setModuleInstance(module.instance);
                if(WASI_API.getModuleInstance()._start) {
                  WASI_API.getModuleInstance()._start();
                } else {
                  throw new Error("failed to load module");
                }
              }
            ).catch(
              function(error){
                throw error;
              }
            );
            }
            return oWasabi;
          }
          function getServerInfo(nId) {
            Promise.resolve(
              null
            ).then(
              function(){
                return ensureWasabi();
              }
            ).then(
              function(){
                try {
                  var wasmRes = WASI_API.getModuleInstance().json_getServerInfo();
                  var res = WASI_API.convertWAsmStr2JSStr(
                    wasmRes, WASI_API.getModuleInstance().memory);
                  main.ResolvePromise(nId, res);
                } catch(error) {
                  main.ResolvePromise(nId, error);
                }
              }
            );
          }
          function getResponse(nId, sBody) {
            Promise.resolve(
              null
            ).then(
              function(){
                return ensureWasabi();
              }
            ).then(
              function(){
                try {
                  var queryWAsm = WASI_API.convertJSStr2WAsm(
                    sBody,
                    WASI_API.getModuleInstance()
                  );
                  var wasmRes = WASI_API.getModuleInstance().json_getResponse_json(queryWAsm);
                  WASI_API.getModuleInstance().free(queryWAsm);
                  var res = WASI_API.convertWAsmStr2JSStr(wasmRes, WASI_API.getModuleInstance().memory);
                  main.ResolvePromise(nId, res);
                } catch(error) {
                  main.ResolvePromise(nId, error);
                }
              }
            ).catch(
              function(error){
                main.ResolvePromise(nId, error);
              }
            );
          }
        },
        [
          "getServerInfo",
          "getResponse"
        ],
        ["ResolvePromise","WriteLog"], [ResolvePromise,WriteLog]
      );
      that.executeServerInfo = function(){
        var nId = n;
        n=n+1;
        oPromHash[nId] = (function(){
          var fRes,fRej;
          function handle(res,rej){
            fRes = res;
            fRej = rej;
          }
          var oProm = new Promise(handle);
          return {
            prom: oProm,
            res: fRes,
            rej: fRej
          };
        }());
        oWorker.getServerInfo(nId);
        return oPromHash[nId].prom;
      };
      that.executeGetResponse = function(sBody){
        var nId = n;
        n=n+1;
        oPromHash[nId] = (function(){
          var fRes,fRej;
          function handle(res,rej){
            fRes = res;
            fRej = rej;
          }
          var oProm = new Promise(handle);
          return {
            prom: oProm,
            res: fRes,
            rej: fRej
          };
        }());
        oWorker.getResponse(nId,sBody);
        return oPromHash[nId].prom;
      };
    };
    return Main;
  }
);
