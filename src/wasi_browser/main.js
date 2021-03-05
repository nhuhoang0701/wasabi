var worker = new Worker("Microcube_worker.js");

ID = 0;
worker.postMessage([ID++, "load"]);

worker.onmessage = function(msg) {
	response_ID = msg.data[0];
	response_Action = msg.data[1];
	
	console.log("worker.onmessage: " + msg.data);
	console.log("worker.onmessage, received msg ID:" + response_ID);
	
	if(response_Action && response_Action.stack && response_Action.message)
	{
		console.error("worker.onmessage, response_ID:'" + response_ID + "' :" + response_Action.message);
	}
	else if(response_Action=="load")
	{
		worker.postMessage([ID++, "GetServerInfo"]);
	}
	else if(response_Action=="GetServerInfo")
	{
		worker.postMessage([ID++, "GetResponse", ["{\"Analytics\":{\"DataSource\":{\"CustomProperties\":{\"cnxString\":\"local:sqlite:efashion.db\"},"
		+"\"ObjectName\":\"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma\",\"PackageName\":\"/\",\"Type\":\"sqlite\"},\"Definition\":{\"Description\":\"Order count\","
		+"\"Dimensions\":[{\"Name\":\"Yr\",\"Axis\":\"Rows\"},{\"Name\":\"Qtr\",\"Axis\":\"Rows\"},{\"Name\":\"agg1_id\",\"Axis\":\"Rows\"}],"
		+"\"DataSource\":{\"PackageName\":\"liquid-sqe\",\"ObjectName\":\"LIQUID_SALES_AV1\"},\"Name\":\"Query35\","
		+"\"ReadMode\":\"BookedAndSpaceAndState\"}},\"Description\":\"Order count\",\"Name\":\"QMDS0035\","
		+"\"TestNumber\":66}"]]);
	}
	else if(response_Action=="GetResponse")
	{
	}
};
