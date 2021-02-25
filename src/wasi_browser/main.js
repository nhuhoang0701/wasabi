var worker = new Worker("Microcube_worker.js");
worker.postMessage(["load"]);

worker.onmessage = function(msg) {
	alert("Receive from Microcube_worker: " + msg.data);
	
	if(msg.data[1] && msg.data[1].stack && msg.data[1].message) {
	} else if(msg.data[0]=="load")
		worker.postMessage(["GetServerInfo"]);
	else if(msg.data[0]=="GetServerInfo")
		worker.postMessage(["GetResponse", ["{\"Analytics\":{\"Definition\":{\"Description\":\"Order count by company and year\","
		+"\"Dimensions\":[{\"Name\":\"CompanyName\",\"Axis\":\"Rows\"},{\"Name\":\"Year\",\"Axis\":\"Rows\"}],"
		+"\"DataSource\":{\"PackageName\":\"liquid-sqe\",\"ObjectName\":\"LIQUID_SALES_AV1\"},\"Name\":\"Query35\","
		+"\"ReadMode\":\"BookedAndSpaceAndState\"}},\"Description\":\"Order count by company and year\",\"Name\":\"QMDS0035\","
		+"\"TestNumber\":66}"]]);
	else if(msg.data[0]=="GetResponse") {
	}
};
