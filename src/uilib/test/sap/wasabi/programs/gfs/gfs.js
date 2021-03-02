/*global sap*/
sap.ui.define(
  [
    "sap/base/Log",
    "jquery.sap.global",
    "sap/ui/core/Component"
  ],
  function (Log,jQuery, Component) {
    "use strict";


    function GFS(Module) {
      var that = this;
      var oDE;
      that.run = function () {
        Module.preRun();
        var oP;
        return Promise.resolve(null).then(
          function(){
            return window.gapi.auth2.getAuthInstance().signIn();
          }
        ).catch(
          function(oError){
            Log.error(oError);
          }
        ).then(
          function(){
            return Module.ff_openWindowPromise(
              "Google Drive Synchronizer"
            );
          }
        ).then(
          function (o) {
            oDE = o;
            oP = jQuery(oDE.parentNode);
            oP.css("height","100%");
            return Component.create(
              {
                name: "programs.gfs.explorer",
                componentData: {
                  startupParameters: {
                  }
                }
              }
            );
          }
        ).then(
          function (oComp) {
            var oCompContainer = new sap.ui.core.ComponentContainer({
              component: oComp,
              height: "100%"
            });
            oCompContainer.addStyleClass("sapUiSizeCompact");
            jQuery(oDE.parentNode).css("white-space", "normal");
            oCompContainer.placeAt(oDE.parentNode, 0);
            oP.on("remove", function () {
              oCompContainer.destroy();
            });
            jQuery(oDE).remove();
          }
        ).catch(
          function (oError) {
            Module.print(oError);
            oP.remove();
          }
        );
      };
    }
    return GFS;
  }
);
