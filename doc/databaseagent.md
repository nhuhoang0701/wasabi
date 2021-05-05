# Server side dev env

 
The server side of the CS env is hosted here: https://github.wdf.sap.corp/cs/proto-cs
 
To get it to work you need a jdk 8+ and maven. Clone the git repository, go to the root folder of the cloned repository and launch:

- `mvn install -PinstallDeps -s config/build/settings.xml` : this installs the cs_logging dependency in the local repository (the only dependency that cannot be rebuild from the sources in nexus) :  beware that **this creates the sapdevelop/m2 folder at the root of the file sytem**, which can lead to some permission issues, you can **change the local repository in config/build/settings.xml**.

- `mvn install -s config/build/settings.xml` : this builds / installs the server's code in the local repository

- `cd proto-cs-tests ; mvn -s ../config/build/settings.xml verify -Pserver` : this starts the server configured with the sqlite db and the javabean provider for stored procedures.

# Client side dev env

The Client side of the POC is hosted here: https://github.wdf.sap.corp/cs/proto-cs-js-client
 
To get it to work:

- first: clone the server side git repository, as the client needs to recompile in JS the protocol messages

- install the protoc compiler, and put it somewhere in the path: https://github.com/protocolbuffers/protobuf/releases

- then clone the js client repo (both proto-cs and proto-cs-js-client folders must be in the same folder) then go down to the root of the proto-cs-js-client repository and:

    ```
    npm init -y
    npm install
    npm install -g --force protobufjs
    npm run build-stubs
    ```

- then...
    - either `npm run build` to build the .js locally,

    - or `npm run start`  to start the webpack proxy, but then you'll have to change values in webpack.config.js to point to the correct server and / or specify what port to run the server on. Please note that in the webpack.config.js, you'll have to modify:

        - devServer.public: this is the public address of the server

        - devServer.port: this is the port the webpack proxy operates on (in the default configuration, it is different from the public adress port because the proxy runs in a docker container, so this is the port in the docker container. The port in the public address is the port as mapped from the container to the host)

        - devServer.proxy: this maps the calls to /sap/cs to the backend server

# Using the CS API in the DBProxy

Server : http://lde-docker-3.dataaccess.c.eu-de-1.cloud.sap:20180/sap/cs.

To get more information on how the CS API is used, see https://github.wdf.sap.corp/cs/proto-cs-js-client/blob/master/test.js.