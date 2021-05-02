function _startWorkerTest() {
	var worker = new Worker("Microcube_worker.js");

	let ID_msg = 0;
	worker.postMessage([ID_msg++, "load", "."]);

	worker.onmessage = function(msg) {
		response_ID = msg.data[0];
		response_Action = msg.data[1];
		response_value = msg.data[2];
		
		console.log("******************************************");
		console.log("Worker: Message executed: ID_msg='" + response_ID + "' Action='" + response_Action + "'");
		
		if(response_value.stack && response_value.message)
		{ 
			console.error("worker.onmessage, ID_msg:'" + response_ID + "' message:" + response_value.message);
		}
		else
		{
			if(response_ID == 0 && response_Action=="load")
			{
				worker.postMessage([ID_msg++, "GetServerInfo", ""]);
			}
			else if(response_ID == 1 && response_Action=="GetServerInfo") 
			{
				worker.postMessage([ID_msg++, "GetResponse", '{"Metadata":{"Context":"Analytics","Language":null,"DataSource":{"Type":"View","ObjectName":"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion","SchemaName":"db","InstanceId":"abc85ce0-eb21-9ac5-52d1-cd23007b89d1"},"Capabilities":["AggregationNOPNULL","AggregationNOPNULLZERO","AttributeHierarchy","AttributeValueLookup","AverageCountIgnoreNullZero","CalculatedDimension","ClientCapabilities","Conditions","DatasourceAtService","DimensionValuehelpProperty","ExceptionAggregationDimsAndFormulas","ExceptionSettings","Exceptions","ExtendedDimensions","ExtendedDimensionsFieldMapping","ExtendedDimensionsJoinColumns","ExtendedDimensionsOuterJoin","HierarchyKeyTextName","HierarchyLevelOffsetFilter","HierarchyPath","HierarchyPathUniqueName","MaxResultRecords","MetadataCubeQuery","MetadataDataCategory","MetadataHierarchyLevels","MetadataHierarchyStructure","MetadataIsDisplayAttribute","Obtainability","OrderBy","ResultSetAxisType","ResultSetInterval","ResultSetState","ReturnedDataSelection","SupportsCalculatedKeyFigures","SupportsComplexFilters","SupportsEncodedResultSet","SupportsEncodedResultSet2","SupportsExtendedSort","SupportsIgnoreExternalDimensions","SupportsMemberValueExceptions","SupportsMemberVisibility","SupportsRestrictedKeyFigures","SupportsSetOperand","TechnicalAxis","Totals","TotalsAfterVisibilityFilter","Variables","VisibilityFilter"],"Expand":["Cube"]}}']);
			}
			else if(response_ID == 2 && response_Action=="GetResponse")
			{
				worker.postMessage([ID_msg++, "GetResponse", '{"Metadata":{"DataSource": {"ObjectName": "Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion"}, "Expand":["Cube"]}}']);
			}
			else if(response_ID == 3 && response_Action=="GetResponse")
			{
				worker.postMessage([ID_msg++, "GetResponse",'{"Analytics":{"DataSource":{"ObjectName":"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"Yr","Axis":"Rows"},{"Name":"Month_name","Axis":"Columns"},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"Sales_revenue", "Aggregation":"SUM"}]}]}}}']);
			}
			else if(response_ID == 4 && response_Action=="GetResponse")
			{
				worker.postMessage([ID_msg++, "GetResponse",'{"Analytics":{"DataSource":{"ObjectName":"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"Yr","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"Sales_revenue", "Aggregation":"SUM"},{"Description":"Measure 1","Name":"Quantity_sold", "Aggregation":"SUM"}]}]}}}']);
			}
		}
	};
}
