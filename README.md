# wasabi

POC to execute InA query in the Browser the goal is to have an engine whic hcan process some computation (blending , scalar function, filtering, aggregation)

To execute it in the browser we will use webassembly https://webassembly.org/
We will use c++ to have more chance in case this go in prouction to integrate some MDS or Cube code
The technical language stack will be : 
  wasi https://wasi.dev/   https://github.com/WebAssembly/wasi-sdk
  libc++ LLVM https://releases.llvm.org/11.0.0/projects/libcxx/docs/
  
 # to set the environment:
 From the wasabi root folder (the folder containing this README.md file):
  - source ./wasabi.sh
 
 # to execute wasm/WASI file on your browser:
 If you want to debug c++ in your browser :
  - Install canary "https://www.google.com/chrome/canary/"
  - Install this exetension https://chrome.google.com/webstore/detail/cc%20%20-devtools-support-dwa/pdcpmagijalfljmkmjngeonclgbbannb
    In the parameter of the extension add a path subtitution rule :
      "/home/$user" -> "\\wsl$\Ubuntu\home\$user" (replace $user by your Linux user) 
 
 From the folder wasabi/src/wasi (open the debug to set the output in the console):
  - $HTTSERVER
  - Open your browser at http://localhost:8080 and select your wasm file
    
 
 # to execute wasm/WASI file on your machine:
 $WASMTIME YOURWASMFILE
 
