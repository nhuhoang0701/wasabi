
# wasabi
POC to execute InA query in the Browser the goal is to have an engine which can process some computation (blending , totaux, scalar function, filtering, aggregation)
Tis engine can would be also be plug to CS client to consume some SQL DataBase in  Live way.

To execute it in the browser we will use webassembly https://webassembly.org/
We will use c++ to have more chance to integrate some existing code MDS / Micro Cube code
The technical language stack will be : 

  wasi        : can be executed in the browser, nodejs or on a machine throw Wasmtime  
                     * https://wasi.dev/  
                     * https://wasmtime.dev/  
                      
  wasi-sdk   :  https://github.com/WebAssembly/wasi-sdk
  
  native c++ :  compiler clang (No Emscripten !)

# WindowsSubsystemLinux
The dev. env. is with Ubuntu on WSL
  - Install unbuntu on windows :
     https://ubuntu.com/wsl
 
  - To install/configure git (and retrieve the source)
     https://teams.microsoft.com/l/entity/com.microsoft.teamspace.tab.wiki/tab::a8896480-ec96-4bc1-91fd-1f3baa4c22b9?context=%7B%22subEntityId%22%3A%22%7B%5C%22pageId%5C%22%3A16%2C%5C%22origin%5C%22%3A2%7D%22%2C%22channelId%22%3A%2219%3Afbca808d7716451fa3cf0a9679cb6970%40thread.tacv2%22%7D&tenantId=42f7676c-f455-423c-82f6-dc2d99791af7

  - Tips : From the wls terminal to open the current linux folder in windows explorer:
       ~/wasabi/src$ explorer.exe .
  
 # to set the environment:
 git clone https://github.wdf.sap.corp/I056617/wasabi.git
 
 From the wasabi root folder (the folder containing this README.md file):
  - source ./wasabi.sh
    This will import all necessaries dependencies , compile and place necessary var. env.
 
 # to execute wasm/WASI file on your browser:
 From the folder wasabi/src/wasi_browser (open the debugger to see the output in the console):
  - $HTTSERVER
  - Open your browser at http://localhost:8080 and select your wasm file.
  
      If you want to debug c++ in your browser :
      - Install canary "https://www.google.com/chrome/canary/"
      - Install this exetension https://chrome.google.com/webstore/detail/cc%20%20-devtools-support-dwa/pdcpmagijalfljmkmjngeonclgbbannb
        In the parameter of the extension add a path subtitution rule :
          "/home/$user" -> "\\wsl$\Ubuntu\home\$user" (replace $user by your Linux user) 
          
          And use Canary as browser and the Canary debugger tools :)
 
 # to execute wasm/WASI file on your machine:
 $WASMTIME YOURWASMFILE
 
