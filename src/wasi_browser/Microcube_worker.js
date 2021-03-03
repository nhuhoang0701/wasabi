isLoaded = false;

const WorkerEvent = {
    eLoad: 'load',
    eGetServerInfo: 'GetServerInfo',
    eGetResponse: 'GetResponse'
}

importScripts('./WASI_API.js');
moduleWASI = null;

onmessage = function(e) {
	var message = e.data;
	console.log("******************************************");
	console.log('Worker: Message received:' + message);
	
	if(message.lenght == 0 || message.lenght > 2) {
		throw 'Worker::onmessage: Bad parameters';
	}
	
	var action = message[0];
	
	console.log('Worker: Action received:' + action);
	switch(action){
    case WorkerEvent.eLoad:
		if(isLoaded == true){
			console.error("Worker: Already loaded");
			postMessage({error: "Already loaded"});
			return;
		}

		const importObject =
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
			
			filesystem = ["../resources/response_getSerververInfo.json",
						 "../resources/response_getResponse_Metadat_expand_cube_catalog.json",
						 "../sqlite/efashion.db"];
			WASI_API.wasabi_initFS(filesystem).then(() => 
			{
				if(getModuleInstance()._start)
				{
					console.log('Worker: execute _start');
					getModuleInstance()._start();
				} else
				{
					console.log('Worker: no _start entry point');
				}
				postMessage([WorkerEvent.eLoad, "Worker: Library well loaded"]);
				isLoaded = true;
			});
		}).catch(error=>{
			console.log('Worker: Library not well loaded: ', error),
			console.log("stack" + error.stack),
			postMessage([WorkerEvent.eLoad, error])
		});

		return;
    case WorkerEvent.eGetServerInfo:
		if(isLoaded == false){
			console.error("Worker: Not loaded");
			postMessage([WorkerEvent.eGetServerInfo, {error: "Not loaded"}]);
			return;
		}

		try {
			var wasmRes = moduleWASI.instance.exports.json_getServerInfo();
			var res = convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);

			console.log([WorkerEvent.eGetServerInfo, 'Worker: GetServerInfo executed']);
			postMessage([WorkerEvent.eGetServerInfo, res]);
		} catch(error) {
			console.log('Worker: eGetServerInfo error: ', error);
			console.log("stack" + error.stack),
			postMessage([WorkerEvent.eGetServerInfo, error]);
		}
		
		return;
    case WorkerEvent.eGetResponse:
		if(isLoaded == false){
			console.error("Worker: Not loaded");
			postMessage([WorkerEvent.eGetResponse, {error: "Not loaded"}]);
			return;
		}

		try {
			var queryJS = message[1];
			
			var queryWAsm = convertJSStr2WAsm(queryJS, moduleWASI);
			var wasmRes = moduleWASI.instance.exports.json_getResponse_json(queryWAsm);
			moduleWASI.instance.exports.free(queryWAsm);
			
			var res = convertWAsmStr2JSStr(wasmRes, moduleWASI.instance.exports.memory);

			console.log([WorkerEvent.eGetResponse, 'Worker: GetServerInfo executed']);
			postMessage([WorkerEvent.eGetResponse, res]);
		} catch(error) {
			console.log('Worker: eGetResponse error: ', error);
			console.log("stack" + error.stack),
			postMessage([WorkerEvent.eGetResponse, error]);
		}
		
		return;
	}
	
			
	throw 'Unknow action:' + action;
}
