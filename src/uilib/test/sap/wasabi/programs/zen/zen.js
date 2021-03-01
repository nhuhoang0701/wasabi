/*
 * ${copyright}
 */
/*global sap*/
sap.ui.define(
  [
    "jquery.sap.global",
    "sap/base/Log",
    "sap/ui/core/Component",
    "sap/zen/dsh/library"
  ],
  function (jQuery, Log,Component) {
    "use strict";
    function Zen(Module) {
      var that = this;
      that.run = function (sSysId, sQuery) {
        Module.preRun();
        var oP;
        return Module.ff_openWindowPromise(
          "Design Studio Web"
        ).then(
          function (oDE) {
            oP = jQuery(oDE.parentNode);
            oP.css("height","100%");
            jQuery(oDE).css("height","100%");
            Component.create({
              name: "sap.zen.dsh.fioriwrapper",
              componentData: {
                startupParameters: {
                  XQUERY: [sQuery],
                  appName: ["0ANALYSIS7"]
                }
              }
            }).then(function(){
              Log.error("Second time");
              sap.zen.DesignStudio.prototype.setRepositoryUrl = function() {
                this.repositoryUrl = "/sap/bc/ui5_ui5/ui2/ushell/resources/sap/zen/dsh/applications/";
              };
              sap.zen.LandscapeUtils.prototype.addSelfBW = function() {
                var loSystem = this.oLandscape.createSystem();
                loSystem.setLanguage(this.language);
                loSystem.setSystemType(sap.firefly.SystemType.BW);
                loSystem.setName("self");
                loSystem.setText("self");
                var oSD = window.ffApp.getSystemLandscape().getSystemDescription(sSysId);
                loSystem.setHost(oSD.getHost());
                loSystem.setPort(oSD.getPort());
                loSystem.setClient(oSD.getClient());
                loSystem.setTimeout(60000);
                loSystem.setAuthenticationType(
                  oSD.getAuthenticationType()
                );
                loSystem.setProtocolType( oSD.getProtocolType());
                this.oLandscape.setSystemByDescription(loSystem);
                return loSystem;
              };
              return Component.create({
                name: "sap.zen.dsh.fioriwrapper",
                componentData: {
                  startupParameters: {
                    XQUERY: [sQuery],
                    appName: ["0ANALYSIS"],
                  }
                }
              });
            }).then(function (oComp) {
              var oCompContainer = new sap.ui.core.ComponentContainer({
                component: oComp,
                height: "100%",
                width: "100%"
              });
              oCompContainer.addStyleClass("sapUiSizeCompact");
              jQuery(oDE.parentNode).css("white-space", "normal");
              oCompContainer.placeAt(oDE.parentNode, 0);
              oP.on("remove", function () {
                oCompContainer.destroy();
              });
              jQuery(oDE).remove();
            });
          }
        ).catch(
          function (oError) {
            Module.print(oError);
            oP.remove();
          }
        );
      };
    }
    return Zen;
  }
);
