sap.ui.define(
    [
        "sap/base/Log",
        "programs/microcube/js/wasm/library",
    ],
    function (Log, mcl) {

        var theLib = null;

        function Program(Module) {
            var that = this;
            that.run = function () {
                return Promise.resolve(null).then(
                    function () {
                        Module.preRun();
                        var memory = new mcl.Memory(2 /*pages*/ );
                        var imports = {};
                        imports.env = {};
                        imports.env.memory = memory.memory;
                        imports.env.memoryBase = 0;
                        imports.env.tableBase = 0;
                        imports.env.table = new WebAssembly.Table({
                            initial: 4,
                            maximum: 100,
                            element: 'anyfunc'
                        });
                        imports.table = new WebAssembly.Table({
                            initial: 5,
                            maximum: 100,
                            element: 'anyfunc'
                        });

                        var moduleLib = new mcl.Library("/wasm_clang/libv_microCube", memory);
                        return moduleLib.loadModule(imports);
                    }).then(
                    function (moduleLib) {
                        var res = moduleLib.executeFct("str_getServerInfo_v");
                        Module.printString(res);
                    }).catch(function (oError) {
                    Module.printString(oError.toString());
                }).then(function () {
                    Module.ff_exit();
                });
            }
        }
        return Program;
    })