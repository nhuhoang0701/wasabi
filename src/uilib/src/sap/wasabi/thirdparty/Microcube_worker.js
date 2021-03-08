(function(){
var isLoaded = false;

var WorkerEvent = {
  eLoad: 'load',
  eGetServerInfo: 'GetServerInfo',
  eGetResponse: 'GetResponse'
}
// https://docs.rs/wasi/0.10.2+wasi-snapshot-preview1/wasi/wasi_snapshot_preview1/index.html

// Members
moduleInstanceExports = null;

var  WASI_CLOCKID_REALTIME                          =  0;
var  WASI_CLOCKID_MONOTONIC                         =  1;
var  WASI_CLOCKID_PROCESS_CPUTIME_ID                =  2;
var  WASI_CLOCKID_THREAD_CPUTIME_ID                 =  3;

function clock_res_realtime () {
  return BigInt(1e6);
};

function clock_res_monotonic() {
  return BigInt(1e3);
};

var clock_res_process = clock_res_monotonic;
var clock_res_thread = clock_res_monotonic;

function clock_time_realtime () {
  return BigInt(Date.now()) * BigInt(1e6);
};

function clock_time_monotonic () {
  var t = performance.now();
  var s = Math.trunc(t);
  var ms = Math.floor((t - s) * 1e3);

  return (BigInt(s) * BigInt(1e9)) + (BigInt(ms) * BigInt(1e6));
};

var clock_time_process = clock_time_monotonic;
var clock_time_thread = clock_time_monotonic;

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
  var heap = new Uint8Array(memory.buffer);
  var str = "";
  for (var i = str_ptr; heap[i] != 0; ++i)
    str += String.fromCharCode(heap[i]);
  return str;
}
function convertJSStr2WAsm(js_str, module)
{
  var heap = new Uint8Array(module.instance.exports.memory.buffer);
  var uint8array = new TextEncoder("utf-8").encode(js_str);
  var size = uint8array.length;
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
WASI_API = {
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
  wasabi_initFS : async function(paths) {
    for (var i=0; i<paths.length; i++)  {
      console.log("4WASI FileSystem: loading... " + paths[i]);
      var response = await fetch(paths[i]);
      arrayBuffer = await response.arrayBuffer();
      var uint8View = new Uint8Array(arrayBuffer);
      fs_Path2Data.set(paths[i], uint8View);
    }
  },
  wasabi_initEnv : async function(environ) {
    env = environ ? environ : {} ;
  },
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
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    return WASI_ENOSYS;
  },
  //*************************************************************
  // path
  path_open: function(dirfd, dirflags, path_ptr, path_len, oflags, fs_rights_base, fs_rights_inheriting, fs_flags, fd) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    vpath  = convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory);
    console.log("vpath:'" + vpath +"'");
    var entry = fds[dirfd];
    if (!entry) {
      console.error('Invalid dirfd');
      return WASI_EBADF;
    } else if (!entry.vpath) {
      console.error('No vpath for dirfd='+dirfd);
      return WASI_EINVAL;
    }
    var text = new TextDecoder();
    var data = new Uint8Array(moduleInstanceExports.memory.buffer, path_ptr, path_len);
    size = fds.length;
    offset = BigInt(0);
    var opened_fd = fds.push({vpath, offset}) - 1;
    var view = new DataView(moduleInstanceExports.memory.buffer);
    view.setUint32(fd, opened_fd, true);
    return WASI_ESUCCESS;
  },
  path_filestat_get: function(fd, flags, path_ptr, path_len, buf) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    console.log("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");
    return WASI_ENOSYS;
  },
  path_unlink_file: function(fd, path_ptr, path_len) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    console.log("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");
    return WASI_ENOSYS;
  },
  poll_oneoff: function(in_, out, nsubscriptions, nevents) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    return WASI_ENOSYS;
  },
  //*************************************************************
  // file descriptor
  fd_sync: function(fd) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    return WASI_ENOSYS;
  },
  fd_seek: function(fd, offset, whence, newoffset_ptr) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    var entry = fds[fd];
    if (!entry) {
      console.error('Invalid fd');
      return WASI_EBADF;
    }
    if(whence == WASI_SEEK_START)
      entry.offset = offset;
    else if(whence == WASI_SEEK_CUR) {
      entry.offset += offset;
    } else if(whence == WASI_SEEK_END) {
      entry.offset += offset;
    } else
      console.error("NYI");
    if(entry.offset < 0)
      entry.offset = BigInt(0);
    var view = new DataView(moduleInstanceExports.memory.buffer);
    view.setBigUint64(newoffset_ptr, BigInt(entry.offset), true);
    return WASI_ESUCCESS;
  },
  fd_write: function(fd, iovs_ptr, iovs_len, nwritten_ptr) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    var entry = fds[fd];
    if (!entry) {
      console.error('Invalid fd');
      return WASI_EBADF;
    } else if (!entry.vpath) {
      console.error('No vpath for fd='+fd);
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
    else console.error("fd_write NYI");
    view.setUint32(nwritten_ptr, written, !0);
    return WASI_ESUCCESS;
  },
  fd_read: function(fd, iovs_ptr, iovs_len, nread_ptr) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    var entry = fds[fd];
    if (!entry) {
      console.error('Invalid fd');
      return WASI_EBADF;
    } else if (!entry.vpath) {
      console.error('No vpath for fd='+fd);
      return WASI_EINVAL;
    }
    if(!entry.data) {
      entry.data = fs_Path2Data.get(entry.vpath);
      if(!entry.data)
      {
        console.error("Please register your file '" + entry.vpath + "' by calling WASI_API::wasabi_initFS()");
        return WASI_EBADF;
      }
      entry.offset = BigInt(0);
    }
    var view = new DataView(moduleInstanceExports.memory.buffer);
    var nread = 0;
    for (var i = 0; i < iovs_len; i++) {
      var data_ptr = view.getUint32(iovs_ptr, true);
      iovs_ptr += 4;
      var data_len = view.getUint32(iovs_ptr, true);
      iovs_ptr += 4;
      var data = new Uint8Array(moduleInstanceExports.memory.buffer, data_ptr, data_len);
      for(var j =0; j < data_len && entry.offset < entry.data.length ; j++) {
        data[j] = entry.data[entry.offset++];
        nread++;
      }
    }
    console.log("char readed:" +nread);
    view.setUint32(nread_ptr, nread, true);
    return WASI_ESUCCESS;
  },
  fd_close: function(fd) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    var entry = fds[fd];
    if (!entry) {
      console.error('Invalid fd');
      return WASI_EBADF;
    }
    fds.splice(fd);
    return WASI_ESUCCESS;
  },
  fd_filestat_get: function(fd, buf_ptr) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    return WASI_ENOSYS;
  },
  fd_fdstat_set_flags: function(fd, flags) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
  },
  fd_fdstat_get: function(fd, stat_ptr) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
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
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    var entry = fds[fd];
    if (!entry) {
      console.error('Invalid fd');
      return WASI_EBADF;
    } else if (!entry.vpath) {
      console.error('No vpath for fd='+fd);
      return WASI_EBADF;
    }
    console.log("vpath:'" + entry.vpath +"'");
    var view = new DataView(moduleInstanceExports.memory.buffer);
    view.setUint8(buf_out, WASI_PREOPENTYPE_DIR);
    view.setUint32(buf_out + 4, new TextEncoder().encode(entry.vpath).byteLength, true);
    return WASI_ESUCCESS;
  },
  fd_prestat_dir_name: function(fd, path_ptr, path_len) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    var entry = fds[fd];
    if (!entry) {
      return WASI_EBADF;
    }
    if (!entry.vpath) {
      return WASI_EBADF;
    }
    console.log("vpath:'" + convertWAsmStr2JSStr(path_ptr, moduleInstanceExports.memory) +"'");
    var data = new Uint8Array(moduleInstanceExports.memory.buffer, path_ptr, path_len);
    data.set(new TextEncoder().encode(entry.vpath));
    return WASI_ESUCCESS;
  },
  //*************************************************************
  // var env
  environ_sizes_get: function(environ_size, environ_buf_size) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    var entries = Object.entries(env);
    var text = new TextEncoder();
    var view = getModuleMemoryDataView();
    view.setUint32(environ_size, entries.length, !0);
    view.setUint32(environ_buf_size, entries.reduce(function(acc, [key, value]) {
      return acc + text.encode(key+"="+value+"\0").length;
    }, 0), true);
    return WASI_ESUCCESS;
  },
  environ_get: function(environ_ptr, environ_buf_ptr) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
    var entries = Object.entries(env);
    var text = new TextEncoder();
    var heap = new Uint8Array(this.memory.buffer);
    var view = getModuleMemoryDataView();
    for (var [key, value] of entries) {
      view.setUint32(environ_ptr, environ_buf_ptr, true);
      environ_ptr += 4;
      var data = text.encode(key+"="+value+"\0");
      heap.set(data, environ_buf_ptr);
      environ_buf_ptr += data.length;
    }
    return WASI_ESUCCESS;
  },
  //*************************************************************
  // Clock
  clock_time_get: function(clock_id, precision, time) {
    console.log("WASI:" + arguments.callee.name + " " + Array.prototype.slice.call(arguments));
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
  }
};

var moduleWASI = null;

onmessage = function(e) {
  var message = e.data;
  console.log("******************************************");
  console.log('Worker: Message received:' + message);
  if(message.lenght == 0 || message.lenght > 2) {
    throw 'Worker::onmessage: Bad parameters';
  }
  var action = message[1];
  var commId = message[0];
  console.log('Worker: Action received:' + action);
  switch(action){
  case WorkerEvent.eLoad:
    if(isLoaded == true){
      console.error("Worker: Already loaded");
      postMessage([commId,new Error( "Already loaded")]);
      return;
    }
    var importObject = {
            wasi_snapshot_preview1: WASI_API,
            env: {},
            js : {mem: new WebAssembly.Memory({initial: 2,maximum: 100})}
          };
    WebAssembly.instantiateStreaming(
      fetch("./InA_Interpreter.wasm"), importObject
    ).then(
      function(module ){
        console.log('Worker: Library well loaded');
        moduleWASI = module;
        setModuleInstance(module.instance);
        filesystem = [
          "./resources/response_getSerververInfo.json",
          "./resources/response_getResponse_Metadat_expand_cube_catalog.json",
          "./sqlite/efashion/efashion.db"
        ];
        WASI_API.wasabi_initFS(
          filesystem
        ).then(function(){
            if(getModuleInstance()._start)
            {
              console.log('Worker: execute _start');
              getModuleInstance()._start();
            } else
            {
              console.log('Worker: no _start entry point');
            }
            postMessage([commId, WorkerEvent.eLoad, "Worker: Library well loaded"]);
            isLoaded = true;
          });
      }
    ).catch(
      function(error){
        postMessage(
          [
            commId,
            WorkerEvent.eLoad,
            error
          ]
        );
      }
    );
    return;
  case WorkerEvent.eGetServerInfo:
    if(isLoaded == false){
      postMessage([commId,WorkerEvent.eGetServerInfo, {error: "Not loaded"}]);
      return;
    }
    try {
      var wasmRes = moduleWASI.instance.exports.json_getServerInfo();
      var res = convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);
      console.log([WorkerEvent.eGetServerInfo, 'Worker: GetServerInfo executed']);
      postMessage(
        [
          commId,
          WorkerEvent.eGetServerInfo,
          res
        ]
      );
    } catch(error) {

      postMessage(
        [
          commId,
          WorkerEvent.eGetServerInfo,
          error
        ]
      );
    }
    return;
  case WorkerEvent.eGetResponse:
    if(isLoaded == false){
      postMessage(
        [
          commId,
          new Error("Not loaded")
        ]
      );
      return;
    }
    try {
      var queryJS = message[2];
      var queryWAsm = convertJSStr2WAsm(queryJS, moduleWASI);
      var wasmRes = moduleWASI.instance.exports.json_getResponse_json(queryWAsm);
      moduleWASI.instance.exports.free(queryWAsm);
      var res = convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);
      console.log([WorkerEvent.eGetResponse, 'Worker: GetServerInfo executed']);
      postMessage([commId,WorkerEvent.eGetResponse, res]);
    } catch(error) {

      postMessage([commId, error]);
    }
    return;
  }
  postMessage(
    [
      commId,
      new Error('Unknow action:' + action)]
  );
}
}());
