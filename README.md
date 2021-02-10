
# wasabi
POC to execute InA query in the Browser the goal is to have an engine which can process some computation (blending , totaux, scalar function, filtering, aggregation)
Tis engine can would be also be plug to CS client to consume some SQL DataBase in  Live way.

To execute it in the browser we will use webassembly https://webassembly.org/
We will use c++ to have more chance to integrate some existing code MDS / Micro Cube code  

The technical stacks are :  
  - [wasi](https://wasi.dev/)        : can be executed in the browser, nodejs or on any machine throw [Wasmtime](https://wasmtime.dev/)      
  - [wasi-sdk](https://github.com/WebAssembly/wasi-sdk)  
  - [clang](https://clang.llvm.org/) compiler (No Emscripten !)  
  - [CMake](https://cmake.org/) for compilation process

# WindowsSubsystemLinux
The dev. env. is tested on Ubuntu in WSL1
  - Install unbuntu on windows :
     https://ubuntu.com/wsl

  - Tips : From the wls terminal to open the current linux folder in windows explorer:  
    <code>~/wasabi/src$ explorer.exe .</code>
  
 # Get the environment:
  
  - install/configure [git](https://teams.microsoft.com/l/entity/com.microsoft.teamspace.tab.wiki/tab::a8896480-ec96-4bc1-91fd-1f3baa4c22b9?context=%7B%22subEntityId%22%3A%22%7B%5C%22pageId%5C%22%3A16%2C%5C%22origin%5C%22%3A2%7D%22%2C%22channelId%22%3A%2219%3Afbca808d7716451fa3cf0a9679cb6970%40thread.tacv2%22%7D&tenantId=42f7676c-f455-423c-82f6-dc2d99791af7) (and retrieve the source)
  - From the ./wasabi root folder (the folder containing this README.md file):  
     `source ./wasabi.sh`  
        This will import all necessaries dependencies , compile and place necessary var. env.
 
 # Execute wasm/WASI file on your browser:
  - From the folder './wasabi/src/wasi_browser' start the http server  by typing `$HTTSERVER`  
  - Open your browser at http://localhost:8080 and select your wasm file  
      for example \\wsl$\Ubuntu\home\ghislain\wasabi5\wasabi\src\test\wasi\build\MyExample.wasm  
      Don't forget to open the browser debugger to see the output in the console.
  - If you want to debug c++ in your browser :
      - Install [canary](https://www.google.com/chrome/canary/)
      - Inside Canary install this [exetension](https://chrome.google.com/webstore/detail/cc%20%20-devtools-support-dwa/pdcpmagijalfljmkmjngeonclgbbannb), in extension option add a path subtitution rule :  
          "/home/$user" -> "\\wsl$\Ubuntu\home\$user" (replace $user by your Linux user) 
          
      - Now use Canary browser and his debugger to debug the c++ code :)
 
 # Execute wasm/WASI file locally on your machine:  
   - `$WASMTIME ./wasabi/src/test/wasi/build/MyExample.wasm`
 
