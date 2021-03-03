moduleWASI = null;

function _handleFiles(fileInput) {
	if (fileInput.length == 0) {
		console.error("Missing file");
		return;
	}
	
	var file = fileInput[0];
	if(file.type !== "application/wasm" ) {
		console.error("File type is not 'application/wasm' but was '" + file.type + "'");
		return;
	}
	let response = new Response(file);
	start(response);
};

function start(response) {

	const importObject =
	{
		wasi_snapshot_preview1: WASI_API,
		env: {},
		js : {mem: new WebAssembly.Memory({initial: 2,maximum: 10})}
	};

	WebAssembly.instantiateStreaming(response, importObject).then(module =>
	{
		moduleWASI = module;
		setModuleInstance(module.instance);
		WASI_API.wasabi_initFS(["../resources/text.txt", "../sqlite/efashion.db", "../resources/response_getResponse_Metadat_expand_cube_catalog.json", "../resources/response_getSerververInfo.json"]).then(() => {
			try {module.instance.exports._start();}
			catch (e)
			{
				console.log("Exception during execution:" + e);
				console.log("stack" + e.stack);
			}
		});
	});
};
