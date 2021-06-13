// Should match  InA_Interpreter.h/Microcube_worker.js
var eLoad = 0;
var eGetServerInfo = 1;
var eGetResponse = 2;
var eSubmitCube = 3;
var eRequestTypeName = ["Load", "GetServerInfo", "GetResponse", "SubmitCube"];

function _startWorkerTest() {
	var worker = new Worker("Microcube_worker.js");

	let ID_msg = 0;
	worker.postMessage([ID_msg++, eLoad, "."]);

	worker.onmessage = function(msg) {
		response_ID = msg.data[0];
		response_Action = msg.data[1];
		response_value = msg.data[2];
		
		WASI_API.wasabi_log("******************************************");
		WASI_API.wasabi_log("Worker_test: Message answer received: ID_msg='" + response_ID + "' Action='" + eRequestTypeName[response_Action] + "'");
		
		if(response_value.stack && response_value.message)
		{ 
			console.error("Worker_test.onmessage, ID_msg:'" + response_ID + "' message:" + response_value.message);
		}
		else
		{
			if(response_ID == 0 && response_Action==eLoad)
			{
				worker.postMessage([ID_msg++, eGetServerInfo, ""]);
			}
			else if(response_ID == 1 && response_Action==eGetServerInfo)
			{
				worker.postMessage([ID_msg++, eGetResponse, '{"Metadata":{"DataSource": {"ObjectName": "$$DataSource$$"}, "Expand":["Cube"]}}']);
			}
			else if(response_ID == 2 && response_Action==eGetResponse)
			{
				worker.postMessage([ID_msg++, eGetResponse, '{"Metadata":{"DataSource": {"ObjectName": "Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion_lite"}, "Expand":["Cube"]}}']);
			}
			else if(response_ID == 3 && response_Action==eGetResponse) 
			{
				worker.postMessage([ID_msg++, eGetResponse, '{"Metadata":{"Context":"Analytics","Language":"EN","DataSource":{"Type":"View","ObjectName":"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion_lite","SchemaName":"db"},"Expand":["Cube"]}}']);
			}
			else if(response_ID == 4 && response_Action==eGetResponse)
			{
				worker.postMessage([ID_msg++, eGetResponse,'{"Analytics":{"DataSource":{"ObjectName":"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion_lite","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"[Year]","Axis":"Rows","Attributes":[{"Name":"Yr"}]},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"Sales_revenue", "Aggregation":"SUM"},{"Description":"Measure 1","Name":"Quantity_sold", "Aggregation":"SUM"}]}]}}}']);
			}
			else if(response_ID == 5 && response_Action==eGetResponse)
			{
				worker.postMessage([ID_msg++, eGetResponse,'{"Analytics":{"DataSource":{"ObjectName":"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion_lite","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"[Year]","Axis":"Rows","Attributes":[{"Name":"Yr"}]},{"Name":"[Month]","Axis":"Columns","Attributes":[{"Name":"Month_name"}]},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"Sales_revenue", "Aggregation":"SUM"}]}]}}}']);
			}
			else
				WASI_API.wasabi_log("Worker_test: Finished");
		}
	};
}
