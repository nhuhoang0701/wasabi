/*!
 * ${copyright}
 */
/*global sap*/
// https://docs.rs/wasi/0.10.2+wasi-snapshot-preview1/wasi/wasi_snapshot_preview1/index.html
sap.ui.define(
  [
    "sap/base/Log"
  ],
  function(Log){
    var Main = function(){
      Log.info("Loaded main");
      var that = this;
      var nCounter = 0;
      var oPromiseMap={};
      that.worker = new Worker(sap.ui.require.toUrl("sap/wasabi/thirdparty/Microcube_worker.js"));
      var oWasabi;
      function ensureWasabi(){
        function handle(res,rej){
          fResolve = res;
          fReject = rej;
        }
        if(!oWasabi){
          nCounter++;
          var fReject, fResolve;
          oWasabi = new Promise(handle);
          oPromiseMap[nCounter] = {
            prom: oWasabi,
            reject: fReject,
            resolve: fResolve
          };
          that.worker.postMessage([nCounter,"load", "."]);
        }
        return oWasabi;
      }
      that.getServerInfo = function(){
        return ensureWasabi().then(
          function(){
            nCounter++;
            var fReject, fResolve;
            function handle(res,rej){
              fResolve = res;
              fReject = rej;
            }
            var oProm = new Promise(handle);
            oPromiseMap[nCounter] = {
              prom: oProm,
              reject: fReject,
              resolve: fResolve
            };
            that.worker.postMessage([nCounter,"GetServerInfo", ""]);
            return oProm;
          }
        );
      };
      that.getResponse = function(requestBody){
        return ensureWasabi().then(
          function(){
            nCounter++;
            var fReject, fResolve;
            function handle(res,rej){
              fResolve = res;
              fReject = rej;
            }
            var oProm = new Promise(handle);
            oPromiseMap[nCounter] = {
              prom: oProm,
              reject: fReject,
              resolve: fResolve
            };
            that.worker.postMessage([nCounter,"GetResponse",requestBody]);
            return oProm;
          }
        );
      };
      that.worker.onmessage = function( data) {
        var msg = event.data;
        var commId = msg[0];
        var oProm = oPromiseMap[commId];
        if(msg[1]&&msg[1]instanceof Error){
          oProm.reject(msg[1]);
        }else{
          oProm.resolve(msg.splice(1));
        }
      };
    };
    return Main;
  }
);
