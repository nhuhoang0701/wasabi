# wasabi

POC to execute InA query in the Browser the goal is to have an engine whic hcan process some computation (blending , scalar function, filtering, aggregation)

To execute it in the browser we will use webassembly https://webassembly.org/
We will use c++ to have more chance in case this go in prouction to integrate some MDS or Cube code
The technical language stack will be : 
  wasi https://wasi.dev/   https://github.com/WebAssembly/wasi-sdk
  libc++ LLVM https://releases.llvm.org/11.0.0/projects/libcxx/docs/
  
  
