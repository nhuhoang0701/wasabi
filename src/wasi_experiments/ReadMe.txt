1. Build wasm : From current dir : 
	- mkdir build & cd build
	- cmake ..
	- make
In build dirrectory, you should have Myexample.wasm

2. Run python : python3 -m http.server 8080
3. Myexample.html

in MyExample.html :
call to _loadWASMFile() function
in comment : call to worker defined in library.js

in MyExample.js (copied from the https://github.wdf.sap.corp/I056617/wasabi/blob/master/src/wasi/polyfill.js) : 
_loadWASMFile() function

