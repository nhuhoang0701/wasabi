var worker = new Worker("Microcube_worker.js");
worker.postMessage(["load"]);

worker.onmessage = function(msg) {
	alert("Receive from Microcube_worker: " + msg.data);
};
