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
    var DisplayResult = function (val, str) {
      // do something here
      Log.error(val);
      Log.error(str);
    };
    var Main = function(){
      var that = this;
      that.execute = function(){
        var oWorker = BuildBridgedWorker(
          function (){
            function CalculateSomething(a, b, c, d) {
              var v = a + b + c + d; //trivial calculation
              main.DisplayResult(v, "hello");
            }
            function CalculateSomethingBig(buff, d) {
              var v = new Uint32Array(buff);
              for (var i = 0; i <= v.length; i++) {
                v[i] /= d;
              }
              main.DisplayResult("big","calculated");
            }
          },
          ["CalculateSomething", "CalculateSomethingBig*"], //note asterisk indicating ArrayBuffer transfer
          ["DisplayResult"], [DisplayResult]
        );
        var w = 9,
            x = 100,
            y = 0,
            z = 2;
        var v = new window.Uint32Array(100);
        oWorker.CalculateSomething(w, x, y, z);
        oWorker.CalculateSomethingBig(v.buffer, x, [v.buffer]);
        return Promise.resolve(null);
      };
    };
    return Main;
  }
);
