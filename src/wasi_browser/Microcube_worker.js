//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Microcube_worker.js


importScripts('./WASI_API.js');

const indexMsgId = 0;
const indexMsgAction = 1;
const indexMsgParam = 2;

function initWasmModule(module, ID, action, param){
	console.log('Worker: initWasmModule');
	WASI_API.setModule(module);
	
	filesystem = [	"/resources/response_getSerververInfo.json",
					"/resources/sqlite/efashion_lite/efashion_lite.db"];
	WASI_API.wasabi_initFS(param, filesystem).then(() => 
	{
		WASI_API.start();
		postMessage([ID, action, '{"message": "Library well loaded"}']);
		return true;
	});
}

(function(){
var isLoaded = false;

var WorkerEvent = {
    eLoad: 'load',
    eGetServerInfo: 'GetServerInfo',
    eGetResponse: 'GetResponse'
}

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
	console.log("Worker: Message received: param");
	try {
		console.log(JSON.parse(param));
	} catch(e) {
		console.log(param);
	}

	let valret = null;
	try {
		switch(action){
		case WorkerEvent.eLoad:
			if(isLoaded == true)
				throw new Error("Worker: Already loaded");

			var importObject =
			{
				wasi_snapshot_preview1: WASI_API,
				env: {},
				js : {mem: new WebAssembly.Memory({initial: 2,maximum: 100})}
			};

			paramDev = "http://localhost:8080";
			fetch(paramDev+"/InA_Interpreter.wasm").then(response =>
					response.arrayBuffer()
				).then(bytes =>
					WebAssembly.instantiate(bytes, importObject)
				).then(module => {
					console.log("Worker: InA_Interpreter.wasm loaded from '" + paramDev +"'");
					isLoaded = initWasmModule(module, ID, action, paramDev);
				}).catch(error=>{
					fetch(param+"/InA_Interpreter.wasm").then(response =>
						response.arrayBuffer()
					).then(bytes =>
						WebAssembly.instantiate(bytes, importObject)
					).then(module => {
						console.log("Worker: InA_Interpreter.wasm loaded from '" + param + "'");
						isLoaded = initWasmModule(module, ID, action, param);
					}).catch(error=>{
						throw error;
					});
			});
/*
			fetch(param+"/InA_Interpreter.wasm").then(response =>
				response.arrayBuffer()
			).then(bytes =>
				WebAssembly.instantiate(bytes, importObject)
			).then(module => {
				console.log("Worker: InA_Interpreter.wasm loaded from '" + param + "'");
				isLoaded = initWasmModule(module, ID, action, param);
			}).catch(error=>{
				throw error;
			});
*/
			return;
		case WorkerEvent.eGetServerInfo:
			if(isLoaded == false)
				throw new Error("Not loaded");

			var wasmRes = WASI_API.getModuleInstanceExports().json_getServerInfo();
			valret = WASI_API.convertWAsmStr2JSStr(wasmRes);		
			break;
		case WorkerEvent.eGetResponse:
			if(isLoaded == false)
				throw new Error("Not loaded");

			var queryJS = message[indexMsgParam];
			var queryWAsm = WASI_API.convertJSStr2WAsm(queryJS);
			var wasmRes = WASI_API.getModuleInstanceExports().json_getResponse_json(queryWAsm);
			WASI_API.getModuleInstanceExports().free(queryWAsm);
			valret = WASI_API.convertWAsmStr2JSStr(wasmRes);
			break;
		default:
			throw  new Error('Unknow action:' + action);
		}
	} catch(error) {
		console.log('Worker: error: ', error);
		console.log("stack: ", error.stack),
		valret = '{"HasErrors":true, "Messages": [{"Number":0,"Type":2,"Text":"'+error.message+'"}]}';
	}
	console.log('Worker: return value:');
	try {
		console.log(JSON.parse(valret));
	} catch(e) {
		console.error(e);
		console.log(valret);
	}
	postMessage([ID, action, valret]);
	
}
}());
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<