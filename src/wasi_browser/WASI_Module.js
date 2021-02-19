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
		try {module.instance.exports._start();}
		catch (e)
		{
			console.log("Exception during execution:" + e);
		}
	});
};
