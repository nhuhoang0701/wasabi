
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

This [Jenkins](https://gkelucjenkins3.jaas-gcp.cloud.sap.corp/view/experiments/job/wasabi_cmake_test/) under construction.  

# WindowsSubsystemLinux
The dev. env. is tested on Ubuntu 20 in WSL1
  - Install unbuntu on windows :
     https://ubuntu.com/wsl  
     Scroll down on this page until: "Install Ubuntu on Windows Subsystem for Linux (WSL)"

  - Tips : From the wls terminal to open the current linux folder in windows explorer:  
    <code>~/explorer.exe .</code>
  
 # Get the environment:
  
  - install/configure [git](https://teams.microsoft.com/l/entity/com.microsoft.teamspace.tab.wiki/tab::a8896480-ec96-4bc1-91fd-1f3baa4c22b9?context=%7B%22subEntityId%22%3A%22%7B%5C%22pageId%5C%22%3A16%2C%5C%22origin%5C%22%3A2%7D%22%2C%22channelId%22%3A%2219%3Afbca808d7716451fa3cf0a9679cb6970%40thread.tacv2%22%7D&tenantId=42f7676c-f455-423c-82f6-dc2d99791af7) (and retrieve the source)
  - From the ~/wasabi root folder (the folder which contains this README.md file):  
     `./wasabi.sh`  
        This will import all necessaries dependencies , compile and place necessary var. env.
        
 Once the env. is ready, to just get the necessaries var. env., from the ~/wasabi root folder (the folder which contains this README.md file):  
      `source ./scripts/set_env.sh`  
 
 # Execute wasm/WASI file on your browser:
  - From the folder '~/wasabi/src/wasi_browser' start the http server  by typing `$HTTP_SERVER`  
  - Open your browser at http://localhost:8080 and select your wasm file  
      for example \\wsl$\Ubuntu\home\ghislain\wasabi5\wasabi\src\test\wasi\build\MyExample.wasm  
      Don't forget to open the browser debugger to see the output in the console.
  - If you want to debug c++ in your browser :
      - Install [canary](https://www.google.com/chrome/canary/)
      - Inside Canary install this [extension](https://chrome.google.com/webstore/detail/cc%20%20-devtools-support-dwa/pdcpmagijalfljmkmjngeonclgbbannb), in extension option add a path subtitution rule :  
          "/home/$user" -> "\\wsl$\Ubuntu\home\\$user" (replace $user by your Linux user) 
      - Enable WebAssemblyDebugging in Canary: https://developers.google.com/web/updates/2020/12/webassembly
      - Now use Canary browser and his debugger to debug the c++ code :)
 
 # Execute wasm/WASI file locally on your machine:  
   - `$WASMTIME ~/wasabi/src/test/wasi/build/MyExample.wasm`
 
# Troubleshooting
  - explorer.exe cannot open WSL directories: https://github.com/microsoft/WSL/issues/4027#issuecomment-496628274
  - WSL must be in version 1.
  You can check it with the following commands:
    wsl --set-default-version 1
    wsl --list -v
  if you see something like
      NAME            STATE           VERSION
    * Ubuntu          Stopped         2
  Then you need to set your version back to 1 with the following command
    wsl --set-version Ubuntu 1
    
