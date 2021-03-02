/*!
 * ${copyright}
 */
/*global sap, main*/
// https://docs.rs/wasi/0.10.2+wasi-snapshot-preview1/wasi/wasi_snapshot_preview1/index.html
sap.ui.define(
  [
    "sap/base/Log",
    "sap/wasabi/BridgedWorker",
    "sap/wasabi/WASI_API"
  ],
  function(Log,BuildBridgedWorker,WASI_API){
    function ensureWasabi(){
      return Promise.resolve(null).then(
        function(){
          var importObject = {
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
          return window.WebAssembly.instantiateStreaming(
            window.fetch(
              "InA_Interpreter.wasm"
            ), importObject
          );
        }
      ).then(
        function(module){
          Log.info("Worker: Library well loaded");
          WASI_API.setModuleInstance(module.instance);
          if(WASI_API.getModuleInstance()._start) {
            Log.info("Worker: execute _start");
            WASI_API.getModuleInstance()._start();
          } else {
            Log.info("Worker: no _start entry point");
          }
        }
      ).catch(
        function(error){
          Log.info("Worker: Library not well loaded: ", error);
          Log.info("stack" + error.stack);
          throw error;
        }
      );
    }
    var n = 0;
    var oPromHash = {};
    var ResolvePromise = function (nId,mValue) {
      var oResolver = oPromHash[nId];
      delete oPromHash[nId];
      ensureWasabi().then(
        function(){
          if( mValue instanceof Error){
            oResolver.rej(mValue);
          }else{
            oResolver.res(mValue);
          }
        }
      ).catch( function(oError){
        oResolver.res(oError);
      });
    };
    var Main = function(){
      var that = this;
      var oWorker = BuildBridgedWorker(
        function (){
          function getServerInfo(nId) {
            main.ResolvePromise(nId, "ServerInfo");
          }
          function getResponse(nId, sBody) {
            main.ResolvePromise(nId,"GetResponse:" + sBody);
          }
        },
        [
          "getServerInfo",
          "getResponse"
        ],
        ["ResolvePromise"], [ResolvePromise]
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
