function _handleFiles(files) {
	if (files.length == 0) {
		console.error("Missing file");
		return;
	}

	for (index = 0; index < files.length; ++index) {
		var file = files[index];
		if(file.type !== "application/wasm" ) {
			console.error("File type is not 'application/wasm' but was '" + file.type + "'");
			return;
		}
		let response = new Response(file);
		_start(response, file.name);
	}
};

function _start(response, filename) {

	const importObject =
	{
		wasi_snapshot_preview1: WASI_API,
		env: {},
		js : {mem: new WebAssembly.Memory({initial: 2,maximum: 10})}
	};

	WebAssembly.instantiateStreaming(response, importObject).then(module =>
	{
		WASI_API.setModule(module);
		filesystem = [];
		WASI_API.wasabi_initFS(".", filesystem).then(() => {
			WASI_API.wasabi_log(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
			WASI_API.wasabi_log(">> start: " + filename);
			try {
				WASI_API.start();
			} catch (e)
			{
				console.log("Exception during execution:" + e);
				console.log("stack" + e.stack);
			}
			WASI_API.wasabi_log("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
		});
	});
};
