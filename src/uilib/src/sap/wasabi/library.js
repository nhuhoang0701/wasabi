/*!
 * ${copyright}
 */
/*global sap*/
/**
 * Initialization Code and shared classes of sap.firefly.fiori library.
 */
sap.ui.define(
  "sap/wasabi/library", [
    "jquery.sap.global",
    "sap/base/Log",
    "sap/m/library"
  ],
  function(
    jQuery, Log,) {
    "use strict";
    /**
     * Wasabi Library which implements a browser based InA server
     *
     * @namespace
     * @name sap.wasabi
     * @author SAP SE
     * @version ${version}
     * @public
     */
    sap.ui.getCore().initLibrary({
      name: "sap.wasabi",
      version: "${version}",
      dependencies: [
        "sap.m",

      ],
      types: [

      ],
      interfaces: [],
      controls: [
      ],
      elements: [
      ],
      models: [
      ],
      i18n: "sap/wasabi/i18n/i18n.properties"
    });

    /**
     * Wasabi Library which implements a browser based InA server
     *
     * @namespace
     * @name sap.wasabi
     * @author SAP SE
     * @version ${version}
     * @public
     */
    var thisLib = sap.wasabi;

    /**
     * Retrieve the Server Info from the embedded wasabi Processor
     * @type {Promise<string>}
     * @public
     * @experimental
     */
    sap.wasabi.getServerInfo = function(){
      var fResolve, fReject;
      function handle(resolve,reject){
        fResolve = resolve;
        fReject = reject;
      }
      var aProm = new Promise(handle);
      fReject(new Error("No Wasabi Worker available"));
      return aProm;
    };
    /**
     * Retrieve the GetResponse from the embedded wasabi Processor
     * @type {Promise<string>}
     * @public
     * @experimental
     */
    sap.wasabi.getResponse = function(sPOSTBody){
      var fResolve, fReject;
      function handle(resolve,reject){
        fResolve = resolve;
        fReject = reject;
      }
      var aProm = new Promise(handle);
      fReject(new Error("No Wasabi Worker available to process: " + sPOSTBody));
      return aProm;
    };

    return thisLib;
  }, /* bExport= */ false);
