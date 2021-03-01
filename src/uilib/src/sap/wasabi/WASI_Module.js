/*!
 * ${copyright}
 */
/*global sap*/
// https://docs.rs/wasi/0.10.2+wasi-snapshot-preview1/wasi/wasi_snapshot_preview1/index.html
sap.ui.define(
  [
    "sap/base/Log",
    "sap/wasabi/WASI_API"
  ],
  function(Log,WASI_API){
    function start(response) {
      var importObject =
          {
            wasi_snapshot_preview1: WASI_API,
            env: {},
            js : {
              mem: new window.WebAssembly.Memory(
                {
                  initial: 2,
                  maximum: 10
                }
              )
            }
          };
      window.WebAssembly.instantiateStreaming(
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
          Log.error("Exception during execution:" + e);
          Log.error("stack" + e.stack);
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
        var response = new window.Response(file);
        start(response);
      }
    };
    return WASIModule;
  }
);
