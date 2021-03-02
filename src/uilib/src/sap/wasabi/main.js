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
        return oPromHash[nId];
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
        return oPromHash[nId];
      };
    };
    return Main;
  }
);
