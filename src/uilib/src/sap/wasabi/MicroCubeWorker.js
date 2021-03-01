/*!
 * ${copyright}
 */
/*global sap*/
// https://docs.rs/wasi/0.10.2+wasi-snapshot-preview1/wasi/wasi_snapshot_preview1/index.html
sap.ui.define(
  [
    "sap/base/Log",
    "sap/wasabi/WASI_API.js"
  ],
  function(Log,WASI_API){
    var isLoaded = false;
    var WorkerEvent = {
      eLoad: "load",
      eGetServerInfo: "GetServerInfo",
      eGetResponse: "GetResponse"
    };
    var moduleWASI = null;
    var onmessage = function(e) {
      var message = e.data;
      Log.info("******************************************");
      Log.info("Worker: Message received:" + message);
      if(message.length == 0 || message.length > 2) {
        throw new Error( "Worker::onmessage: Bad parameters");
      }
      var action = message[0];
      Log.info("Worker: Action received:" + action);
      switch(action){
      case WorkerEvent.eLoad:
        if(isLoaded == true){
          Log.error("Worker: Already loaded");
          window.postMessage({error: "Already loaded"});
          return;
        }
        var importObject =
              {
                wasi_snapshot_preview1: WASI_API,
                env: {},
                js : {
                  mem: new window.WebAssembly.Memory(
                    {
                      initial: 2
                      ,maximum: 10
                    }
                  )
                }
              };
        window.WebAssembly.instantiateStreaming(
          window.fetch(
            "./InA_Interpreter.wasm"
          ), importObject
        ).then(
          function(module){
            Log.info("Worker: Library well loaded");
            moduleWASI = module;
            WASI_API.setModuleInstance(module.instance);
            if(WASI_API.getModuleInstance()._start) {
              Log.info("Worker: execute _start");
              WASI_API.getModuleInstance()._start();
            } else {
              Log.info("Worker: no _start entry point");
            }
            window.postMessage([WorkerEvent.eLoad, "Worker: Library well loaded"]);
            isLoaded = true;
          }
        ).catch(
          function(error){
            Log.info("Worker: Library not well loaded: ", error),
            Log.info("stack" + error.stack),
            window.postMessage([WorkerEvent.eLoad, error]);
          }
        );

        return;
      case WorkerEvent.eGetServerInfo:
        if(isLoaded == false){
          Log.error("Worker: Not loaded");
          window.postMessage([WorkerEvent.eGetServerInfo, {error: "Not loaded"}]);
          return;
        }
        try {
          var wasmRes = moduleWASI.instance.exports.json_getServerInfo();
          var res = WASI_API.convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);
          Log.info([WorkerEvent.eGetServerInfo, "Worker: GetServerInfo executed"]);
          window.postMessage([WorkerEvent.eGetServerInfo, res]);
        } catch(error) {
          Log.info("Worker: eGetServerInfo error: ", error);
          Log.info("stack" + error.stack),
          window.postMessage(
            [
              WorkerEvent.eGetServerInfo,
              error
            ]
          );
        }
        return;
      case WorkerEvent.eGetResponse:
        if(isLoaded == false){
          Log.error("Worker: Not loaded");
          window.postMessage([WorkerEvent.eGetResponse, {error: "Not loaded"}]);
          return;
        }
        try {
          var queryJS = message[1];

          var queryWAsm = WASI_API.convertJSStr2WAsm(queryJS, moduleWASI);
          wasmRes = moduleWASI.instance.exports.json_getResponse_json(queryWAsm);
          moduleWASI.instance.exports.free(queryWAsm);
          res = WASI_API.convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);
          Log.info([WorkerEvent.eGetResponse, "Worker: GetServerInfo executed"]);
          window.postMessage([WorkerEvent.eGetResponse, res]);
        } catch(error) {
          Log.info("Worker: eGetResponse error: ", error);
          Log.info("stack" + error.stack),
          window.postMessage([WorkerEvent.eGetResponse, error]);
        }
        return;
      }
      throw new Error("Unknow action:" + action);
    };
    var MicroCubeWorker = {
      onMessage: onmessage
    };
    return MicroCubeWorker;
  }
);
