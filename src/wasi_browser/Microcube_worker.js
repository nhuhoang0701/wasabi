isLoaded = false;

const WorkerEvent = {
    eLoad: 'load',
    eGetServerInfo: 'GetServerInfo',
    eGetResponse: 'GetResponse'
}

importScripts('./WASI_API.js');

moduleG = null;

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
			js : {mem: new WebAssembly.Memory({initial: 2,maximum: 10})}
		};
		WebAssembly.instantiateStreaming(fetch("InA_Interpreter.wasm"), importObject).then(module =>
		{
			console.log('Worker: Library well loaded');
			moduleG = module;
			setModuleInstance(module.instance);
			
			if(getModuleInstance()._start) {
				console.log('Worker: execute _start');
				getModuleInstance()._start();
			} else {
				console.log('Worker: no _start entry point');
			}
			postMessage([WorkerEvent.eLoad, "Worker: Library well loaded"]);
			isLoaded = true;
		}).catch(error=>{
			console.log('Worker: Library not well loaded: ', error),
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
			var res = getJSStringFromWAsmAt(moduleG.instance.exports.json_getServerInfo(), moduleG.instance.exports.memory);

			console.log([WorkerEvent.eGetServerInfo, 'Worker: GetServerInfo executed']);
			postMessage([WorkerEvent.eGetServerInfo, res]);
		} catch(error) {
			console.log('Worker: eGetServerInfo error: ', error);
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
			var query = message[1];
			var res = getJSStringFromWAsmAt(moduleG.instance.exports.json_getResponse_json(query), moduleG.instance.exports.memory);

			console.log([WorkerEvent.eGetResponse, 'Worker: GetServerInfo executed']);
			postMessage([WorkerEvent.eGetResponse, res]);
		} catch(error) {
			console.log('Worker: eGetResponse error: ', error);
			postMessage([WorkerEvent.eGetResponse, error]);
		}
		
		return;
	}
	
			
	throw 'Unknow action:' + action;
}
