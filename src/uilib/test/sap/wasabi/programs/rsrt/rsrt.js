/*
 * ${copyright}
 */
/*global sap*/
sap.ui.define(
  [
    "jquery.sap.global",
    "sap/ui/core/Component"
  ],
  function (jQuery, Component) {
    "use strict";
    function RsRt(Module) {
      var that = this;
      that.run = function () {
        Module.preRun();
        var oP;
        return Module.ff_openWindowPromise(
          "RSRT FireFly"
        ).then(
          function (oDE) {
            oP = jQuery(oDE.parentNode);
            Component.create({
              name: "sap.firefly.fiori.rsrt",
              componentData: {
                startupParameters: {
                }
              }
            }).then(function (oComp) {
              var oCompContainer = new sap.ui.core.ComponentContainer({
                component: oComp,
                height:"100%",
                width: "100%",
              });
              oCompContainer.addStyleClass("sapUiSizeCompact");
              jQuery(oDE.parentNode).css("white-space", "normal");
              oCompContainer.placeAt(oDE.parentNode, 0);
              oP.on("remove", function () {
                oCompContainer.destroy();
              });
              jQuery(oDE).remove();
            });
          }).catch(function (oError) {
            Module.print(oError);
            oP.remove();
          });
      };
    }
    return RsRt;
  }
)
