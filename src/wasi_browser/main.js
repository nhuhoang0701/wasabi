var worker = new Worker("Microcube_worker.js");
worker.postMessage(["load"]);

worker.onmessage = function(msg) {
	alert("Receive from Microcube_worker: " + msg.data);
	
	if(msg.data[1] && msg.data[1].stack && msg.data[1].message) {
	} else if(msg.data[0]=="load")
		worker.postMessage(["GetServerInfo"]);
	else if(msg.data[0]=="GetServerInfo")
		worker.postMessage(["GetResponse", ["{\"not an InA\":0}"]]);
	else if(msg.data[0]=="GetResponse") {
	}
};
