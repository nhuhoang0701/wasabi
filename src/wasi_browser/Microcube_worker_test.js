function _startWorkerTest() {
	var worker = new Worker("Microcube_worker.js");

	ID = 0;
	worker.postMessage([ID++, "load", "."]);

	worker.onmessage = function(msg) {
		response_ID = msg.data[0];
		response_Action = msg.data[1];
		response_value = msg.data[2];
		
		console.log("******************************************");
		console.log("Worker: Message executed: ID='" + response_ID + "' Action='" + response_Action + "'");
		
		if(response_value.stack && response_value.message)
		{ 
			console.error("worker.onmessage, response_ID:'" + response_ID + "' message:" + response_value.message);
		}
		else
		{
			console.log("Worker: Message executed: value='" + response_value.substring(0, 150) + "'");
			if(response_ID == 0 && response_Action=="load")
			{
				worker.postMessage([ID++, "GetServerInfo", ""]);
			}
			else if(response_ID == 1 && response_Action=="GetServerInfo")
			{
				worker.postMessage([ID++, "GetResponse", '{"Metadata":{"DataSource": {"ObjectName": "$$DataSource$$","PackageName":"local:sqlite:efashion"}, "Expand":["Cubes"]}}']);
			}
			else if(response_ID == 2 && response_Action=="GetResponse")
			{
				worker.postMessage([ID++, "GetResponse", '{"Metadata":{"DataSource": {"ObjectName": "Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion"}, "Expand":["Cubes"]}}']);
			}
			else if(response_ID == 3 && response_Action=="GetResponse")
			{
				worker.postMessage([ID++, "GetResponse",'{"Analytics":{"DataSource":{"ObjectName":"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"Yr","Axis":"Rows"},{"Name":"Month_name","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"Sales_revenue", "Aggregation":"SUM"}]}]}}}']);
			}
			else if(response_ID == 4 && response_Action=="GetResponse")
			{
				worker.postMessage([ID++, "GetResponse",'{"Analytics":{"DataSource":{"ObjectName":"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"Yr","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"Sales_revenue", "Aggregation":"SUM"},{"Description":"Measure 1","Name":"Quantity_sold", "Aggregation":"SUM"}]}]}}}']);
			}
		}
	};
}
