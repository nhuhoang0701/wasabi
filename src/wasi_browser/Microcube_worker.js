//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Microcube_worker.js


importScripts('./WASI_API.js');
moduleWASI = null;

const indexMsgId = 0;
const indexMsgAction = 1;
const indexMsgParam = 2;

function initWasmModule(module, ID, action, param){
	console.log('Worker: initWasmModule');
	moduleWASI = module;
	setModuleInstance(module.instance);
	
	filesystem = [	"/resources/response_getSerververInfo.json",
					"/resources/response_getResponse_Metadata_expand_cube_catalog.json",
					"/resources/sqlite/efashion/efashion.db"];
	WASI_API.wasabi_initFS(param, filesystem).then(() => 
	{
		if(getModuleInstance()._initialize)
		{
			console.log('Worker: execute _initialize');
			getModuleInstance()._initialize();
		}
		else if(getModuleInstance()._start)
		{
			console.log('Worker: execute _start');
			getModuleInstance()._start();
		}
		else
		{
			console.log('Worker: no _start entry point');
		}
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

			fetch("http://localhost:8080/InA_Interpreter.wasm").then(response =>
					response.arrayBuffer()
				).then(bytes =>
					WebAssembly.instantiate(bytes, importObject)
				).then(module => {
					console.log("Worker: InA_Interpreter.wasm loaded from localhost:8080");
					isLoaded = initWasmModule(module, ID, action, "http://localhost:8080");
				}).catch(error=>{
					fetch(param+"/InA_Interpreter.wasm").then(response =>
						response.arrayBuffer()
					).then(bytes =>
						WebAssembly.instantiate(bytes, importObject)
					).then(module => {
						console.log("Worker: InA_Interpreter.wasm loaded from " + param);
						isLoaded = initWasmModule(module, ID, action, param);
					}).catch(error=>{
						throw error;
					});
			});

			return;
		case WorkerEvent.eGetServerInfo:
			if(isLoaded == false)
				throw new Error("Not loaded");

			var wasmRes = moduleWASI.instance.exports.json_getServerInfo();
			valret = convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);		
			break;
		case WorkerEvent.eGetResponse:
			if(isLoaded == false)
				throw new Error("Not loaded");

			var queryJS = message[indexMsgParam];
			var queryWAsm = convertJSStr2WAsm(queryJS, moduleWASI);
			var wasmRes = moduleWASI.instance.exports.json_getResponse_json(queryWAsm);
			moduleWASI.instance.exports.free(queryWAsm);
			valret = convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);			
			break;
		default:
			throw  new Error('Unknow action:' + action);
		}
	} catch(error) {
		console.log('Worker: error: ', error);
		console.log("stack: ", error.stack),
		valret = '{"Messages": [{"Number":0,"Type":"Error","Text":"'+error+'"}]}';
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