/*!
 * ${copyright}
 */
/*global sap*/
sap.ui.define(
    [
        "sap/base/Log"
    ],
    function (Log) {
        "use string";

        function gnuplot(jsname) {
            var that = this;
            var aDefers = [];
            var output = [];
            var error = [];
            var worker = null;
            var transaction = 1;
            var isRunning = false;
            worker = new Worker(jsname);
            worker.addEventListener('message', function (e) {
                var oData = e.data;
                Log.info("finished transcation:" + oData.transaction);
                if (oData.transaction < 0) {
                    if (oData.transaction == -1) {
                        output.push(oData.content);
                        that.onOutput(oData.content);
                    };
                    if (oData.transaction == -2) {
                        error.push(oData.content);
                        that.onError(oData.content);
                    };
                    return;
                }
                if (oData.content == 'FINISH')
                    isRunning = false;
                if (oData.transaction) {
                    var oDefer = aDefers[oData.transaction];
                    delete aDefers[oData.transaction];
                    oDefer.resolve(oData.content)
                }
            }, false);
            worker.postMessage({});
            that.putFile = function (sName, oContents) {
                Log.info("Put File:" + sName);
                return postCommand({
                    name: sName,
                    content: oContents,
                    cmd: 'putFile'
                }, null);
            };
            that.getFile = function (sName) {
                Log.info("Get File:" + sName);
                return postCommand({
                    name: sName,
                    cmd: 'getFile'
                });
            };

            // remove file
            that.removeFile = function (sName) {
                Log.info("Remove File:" + sName);
                return postCommand({
                    name: sName,
                    cmd: 'removeFile'
                });
            };

            that.getListing = function () {
                Log.info("Get listing:");
                return postCommand(data = {
                    cmd: 'getListing'
                });
            };
            that.run = function (sScript) {
                Log.info("Run Script:" + sScript);
                if (isRunning) {
                    return Promise.reject(new Error("Already running"));
                }
                isRunning = true;
                return postCommand({
                    content: sScript,
                    cmd: 'run'
                });

            };
            that.onOutput = function (text) {
                Log.info("OUT" + text);
            };
            that.onError = function (text) {
                Log.error("ERR" + text);
            };

            function postCommand(oData) {

                var id = transaction; //fresh id
                oData.transaction = id; // give data object a tag
                Log.info("Post transcation:" + id);

                function Defer() {
                    var that = this;

                    function handleDialog(resolve, reject) {
                        that.resolve = resolve;
                        that.reject = reject;
                    }
                    that.promise = new Promise(handleDialog)

                }
                aDefers[id] = new Defer();
                worker.postMessage(oData);
                transaction++;
                return aDefers[id].promise
            }
        };
        return new gnuplot('programs/plot/gnuplot.js');
    });