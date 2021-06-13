//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Microcube_worker.js


importScripts('./WASI_API.js');

// Should match  InA_Interpreter.h/Microcube_worker.js
var eLoad = 0;
var eGetServerInfo = 1;
var eGetResponse = 2;
var eSubmitCube = 3;
var eRequestTypeName = ["Load", "GetServerInfo", "GetResponse", "SubmitCube"];

const indexMsgId = 0;
const indexMsgAction = 1;
const indexMsgParam = 2;

function initWasmModule(module, ID, action, param){
	console.log('Worker: initWasmModule');
	WASI_API.setModule(module);

	filesystem = [
	];
	WASI_API.wasabi_initFS(param, filesystem).then(() => 
	{
		WASI_API.start();
		postMessage([ID, action, '{"message": "Library well loaded"}']);
		return true;
	});
}

(function(){
var isLoaded = false;



function ina_callback_response(ID, type, inaResponse) {
	console.log("****  ina_callback_response  *****");
	console.log("Worker: Message executed: ID '" + ID + "' Type:'" + eRequestTypeName[type] + "'");
	console.log("Worker: Message executed: response");

	let js_inaResponse = WASI_API.convertWAsmStr2JSStr(inaResponse);
	try {
		console.log(JSON.parse(js_inaResponse));
	} catch(e) {
		console.error(e);
		console.log(js_inaResponse);
	}
	postMessage([ID, type, js_inaResponse]);
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
	console.log("Worker: Message received: ID '" + ID + "' Action:'" + eRequestTypeName[action] + "'");
	console.log("Worker: Message received: param");
	try {
		console.log(JSON.parse(param));
	} catch(e) {
		console.log(param);
	}

	try {
		switch(action){
		case eLoad:
			if(isLoaded == true)
				throw new Error("Worker: Already loaded");

			let imports = {ina_callback_response:ina_callback_response};
			var importObject =
			{
				wasi_snapshot_preview1: WASI_API,
				env: imports,
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
		case eGetServerInfo:
			if(isLoaded == false)
				throw new Error("Not loaded");

			WASI_API.getModuleInstanceExports().doIt(ID, eGetServerInfo, null);
			

			if(WASI_API.getModuleInstanceExports().asyncify_get_state) {
				if(WASI_API.getModuleInstanceExports().asyncify_get_state()==1) {
					WASI_API.getModuleInstanceExports().asyncify_stop_unwind();
					WASI_API.wasabi_log('2 asyncify_stop_unwind(ed), state:'+WASI_API.getModuleInstanceExports().asyncify_get_state());
				}
			}
			break;
		case eGetResponse:
			if(isLoaded == false)
				throw new Error("Not loaded");

			var queryJS = message[indexMsgParam];
			var queryWAsm = WASI_API.convertJSStr2WAsm(queryJS);
			WASI_API.getModuleInstanceExports().doIt(ID, eGetResponse, queryWAsm);
			WASI_API.getModuleInstanceExports().free(queryWAsm);
			break;
		default:
			throw  new Error('Unknow action:' + action);
		}
	} catch(error) {
		console.log('Worker: error: ', error);
		console.log("stack: ", error.stack);
		let valret = '{"HasErrors":true, "Messages": [{"Number":0,"Type":2,"Text":"'+error.message+'"}]}';
		console.log('Worker: return value:');
		try {
			console.log(JSON.parse(valret));
		} catch(e) {
			console.error(e);
			console.log(valret);
		}
		postMessage([ID, action, valret]);
	}
}
}());
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<