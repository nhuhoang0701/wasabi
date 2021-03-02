var worker = new Worker("Microcube_worker.js");
worker.postMessage(["load"]);

worker.onmessage = function(msg) {
	//alert("Receive from Microcube_worker: " + msg.data);
	
	if(msg.data[1] && msg.data[1].stack && msg.data[1].message) {
	} else if(msg.data[0]=="load")
		worker.postMessage(["GetServerInfo"]);
	else if(msg.data[0]=="GetServerInfo")
		worker.postMessage(["GetResponse", ["{\"Analytics\":{\"DataSource\":{\"CustomProperties\":{\"cnxString\":\"local:sqlite:efashion.db\"},"
		+"\"ObjectName\":\"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma\",\"PackageName\":\"/\",\"Type\":\"sqlite\"},\"Definition\":{\"Description\":\"Order count\","
		+"\"Dimensions\":[{\"Name\":\"Yr\",\"Axis\":\"Rows\"},{\"Name\":\"Qtr\",\"Axis\":\"Rows\"},{\"Name\":\"agg1_id\",\"Axis\":\"Rows\"}],"
		+"\"DataSource\":{\"PackageName\":\"liquid-sqe\",\"ObjectName\":\"LIQUID_SALES_AV1\"},\"Name\":\"Query35\","
		+"\"ReadMode\":\"BookedAndSpaceAndState\"}},\"Description\":\"Order count\",\"Name\":\"QMDS0035\","
		+"\"TestNumber\":66}"]]);
	else if(msg.data[0]=="GetResponse") {
	}
};
