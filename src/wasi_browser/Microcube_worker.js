//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Microcube_worker.js
(function(){
var isLoaded = false;

var WorkerEvent = {
    eLoad: 'load',
    eGetServerInfo: 'GetServerInfo',
    eGetResponse: 'GetResponse'
}

importScripts('./WASI_API.js');
moduleWASI = null;

const indexMsgId = 0;
const indexMsgAction = 1;
const indexMsgParam = 2;

onmessage = function(e) {
	var message = e.data;
	if(message.length != 3) {
		throw 'Worker::onmessage: Expect 3 parameters';
	}

	var ID = message[indexMsgId];
	var action = message[indexMsgAction];
	var param = message[indexMsgParam];

	console.log("******************************************");
	console.log("Worker: Message received: ID '" + ID + "' Action:'" + action + "'");
	console.log("Worker: Message received: param='" + param.substring(0, 150) + "'");
	

	switch(action){
    case WorkerEvent.eLoad:
		if(isLoaded == true){
			console.error("Worker: Already loaded");
			postMessage([ID, action, {error: "Already loaded"}]);
			return;
		}

		var importObject =
		{
			wasi_snapshot_preview1: WASI_API,
			env: {},
			js : {mem: new WebAssembly.Memory({initial: 2,maximum: 100})}
		};
		WebAssembly.instantiateStreaming(fetch("./InA_Interpreter.wasm"), importObject).then(module =>
		{
			console.log('Worker: Library well loaded');
			moduleWASI = module;
			setModuleInstance(module.instance);
			
			filesystem = ["/resources/response_getSerververInfo.json",
						 "/resources/response_getResponse_Metadat_expand_cube_catalog.json",
						 "/resources/sqlite/efashion/efashion.db"];
			WASI_API.wasabi_initFS(param, filesystem).then(() => 
			{
				if(getModuleInstance()._start)
				{
					console.log('Worker: execute _start');
					getModuleInstance()._start();
				} else
				{
					console.log('Worker: no _start entry point');
				}
				postMessage([ID, WorkerEvent.eLoad, "Worker: Library well loaded"]);
				isLoaded = true;
			});
		}).catch(error=>{
			console.log('Worker: Library not well loaded: ', error),
			console.log("stack" + error.stack),
			postMessage([ID, WorkerEvent.eLoad, error])
		});

		return;
    case WorkerEvent.eGetServerInfo:
		if(isLoaded == false){
			console.error("Worker: Not loaded");
			postMessage([ID, WorkerEvent.eGetServerInfo, {error: "Not loaded"}]);
			return;
		}

		try {
			var wasmRes = moduleWASI.instance.exports.json_getServerInfo();
			var res = convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);

			console.log([WorkerEvent.eGetServerInfo, 'Worker: GetServerInfo executed']);
			postMessage([ID, WorkerEvent.eGetServerInfo, res]);
		} catch(error) {
			console.log('Worker: eGetServerInfo error: ', error);
			console.log("stack" + error.stack),
			postMessage([ID, WorkerEvent.eGetServerInfo, error]);
		}
		
		return;
    case WorkerEvent.eGetResponse:
		if(isLoaded == false){
			console.error("Worker: Not loaded");
			postMessage([ID, WorkerEvent.eGetResponse, {error: "Not loaded"}]);
			return;
		}

		try {
			var queryJS = message[indexMsgParam];
			
			var queryWAsm = convertJSStr2WAsm(queryJS, moduleWASI);
			var wasmRes = moduleWASI.instance.exports.json_getResponse_json(queryWAsm);
			moduleWASI.instance.exports.free(queryWAsm);
			
			var res = convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);

			console.log([WorkerEvent.eGetResponse, 'Worker: GetServerInfo executed']);
			postMessage([ID, WorkerEvent.eGetResponse, res]);
		} catch(error) {
			console.log('Worker: eGetResponse error: ', error);
			console.log("stack" + error.stack),
			postMessage([ID, WorkerEvent.eGetResponse, error]);
		}
		
		return;
	}
	
	postMessage([ID, action, new Error('Unknow action:' + action)]);
}
}());
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<