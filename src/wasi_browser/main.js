var worker = new Worker("Microcube_worker.js");

ID = 0;
worker.postMessage([ID++, "load", ""]);

worker.onmessage = function(msg) {
	response_ID = msg.data[0];
	response_Action = msg.data[1];
	response_value = msg.data[2];
	
	console.log("******************************************");
	console.log("Worker: Message executed: ID='" + response_ID + "' Action='" + response_Action + "'");
	console.log("Worker: Message executed: value='" + response_value.substring(0, 150) + "'");
	
	if(response_Action && response_Action.stack && response_Action.message)
	{
		console.error("worker.onmessage, response_ID:'" + response_ID + "' :" + response_Action.message);
	}
	else if(response_ID == 0 && response_Action=="load")
	{
		worker.postMessage([ID++, "GetServerInfo", ""]);
	}
	else if(response_ID == 1 && response_Action=="GetServerInfo")
	{
		worker.postMessage([ID++, "GetResponse", '{"Metadata":{"DataSource": {"ObjectName": "$$DataSource$$","PackageName":"local:sqlite:onetable_datatype.db"}, "Expand":["Cubes"]}}']);
	}
	else if(response_ID == 2 && response_Action=="GetResponse")
	{
		worker.postMessage([ID++, "GetResponse", '{"Metadata":{"DataSource": {"ObjectName": "onetable_datatype","PackageName":"local:sqlite:onetable_datatype.db"}, "Expand":["Cubes"]}}']);
	}
	else if(response_ID == 3 && response_Action=="GetResponse")
	{
		worker.postMessage([ID++, "GetResponse",'{"Analytics":{"DataSource":{"ObjectName":"onetable_datatype","PackageName":"local:sqlite:onetable_datatype.db","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"text","Axis":"Rows"},{"Name":"varchar","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Cols","Members":[{"Description":"Measure 1","Name":"real", "Aggregation":"SUM"}]}]}}}']);
	}
	else if(response_ID == 4 && response_Action=="GetResponse")
	{
		worker.postMessage([ID++, "GetResponse",'{"Analytics":{"DataSource":{"ObjectName":"onetable_datatype","PackageName":"local:sqlite:onetable_datatype.db","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"varchar","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Cols","Members":[{"Description":"Measure 1","Name":"real", "Aggregation":"SUM"},{"Description":"Measure 1","Name":"integer", "Aggregation":"SUM"}]}]}}}']);
	}
};
