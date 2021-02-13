isLoaded = false;

const WorkerEvent = {
    eLoad: 'load',
    eGetServerInfo: 'GetServerInfo',
    eGetResponse: 'GetResponse'
}

importScripts('./WASI_API.js');

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
		isLoaded = true;

		const importObject =
		{
			wasi_snapshot_preview1: WASI_API,
			env: {},
			js : {mem: new WebAssembly.Memory({initial: 2,maximum: 10})}
		};
		WebAssembly.instantiateStreaming(fetch("MyExample.wasm"), importObject).then(module =>
		{
			console.log('Worker: Library well loaded');
			WASI_API.setModuleInstance(module.instance);
			
			if(module.instance.exports._start) {
				console.log('Worker: execute _start');
				module.instance.exports._start();
			} else {
				console.log('Worker: no _start entry point');
			}
			postMessage("Worker: Library well loaded");
		}).catch(error=>{
			console.log('Worker: Library not well loaded: ', error)
		});

		return;
    case WorkerEvent.eGetServerInfo:
		if(isLoaded == false){
			console.error("Worker: Not loaded");
			postMessage({error: "Not loaded"});
			return;
		}

		var res = moduleLib.executeFct("json_getServerInfo");

		console.log('Worker: GetServerInfo executed');
		postMessage(res);
		
		return;
    case WorkerEvent.eGetResponse:
		if(isLoaded == false){
			console.error("Worker: Not loaded");
			postMessage({error: "Not loaded"});
			return;
		}

		var query = message[1];
		var res = moduleLib.executeFct("json_getResponse_json", query);

		console.log('Worker: GetServerInfo executed');
		postMessage(res);
		
		return;
	}
	
			
	throw 'Unknow action:' + action;
}
