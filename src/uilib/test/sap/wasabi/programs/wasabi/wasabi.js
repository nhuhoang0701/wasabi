/*
 * ${copyright}
 */
/*global sap*/
sap.ui.define(
  [
    "sap/base/Log",
    "jquery.sap.global",
    "sap/m/TabContainer",
    "sap/m/TabContainerItem",
    "sap/ui/codeeditor/CodeEditor",
    "sap/m/Button",
    "sap/m/Image",
    "sap/wasabi/main"
  ],
  function (
    Log, jQuery, TabContainer, TabContainerItem, CodeEditor, Button, Image, Main
  ) {
    var getCount = (
      function () {
        var n = 0;
        return function () {
          return ++n;
        };
      }
      ()
    );
    function Program(Module) {
      var that = this;
      that.run = function () {
        Module.preRun();
        return Module.ff_openWindowPromise(
          "wasabi"
        ).then(
          function (oDE) {
            var oVBox = new TabContainer();
            oVBox.placeAt(oDE.parentNode, 0);
            jQuery(oDE).remove();
            var oItem = new TabContainerItem({
              name: "Request Body"
            });
            oVBox.addItem(oItem);
            var oTBData = new CodeEditor(
              {
                height: "300px",
                type: "text",
                width: "100%"
              }
            );
            oItem.addContent(oTBData);
            oItem = new TabContainerItem({
              name: "Response Body"
            });
            oVBox.addItem(oItem);
            var oTBPlot = new CodeEditor({
              height: "300px",
              type: "text",
              width: "100%"
            });
            oItem.addContent(oTBPlot);
            oVBox.addItem(oItem);

            var oButton = new Button({
              text: "Save"
            });
            oButton.attachPress(function () {
              var sName = ["/", "PlotImage(" + getCount() + ")", ".svg"].join("");
              Module.writeVfsFile(sName, window._mimestring || "empty");
            });
            oItem.addContent(oButton);
            oVBox.attachItemSelect(function () {
              oVBox.setBusy(true);
              return Promise.resolve(
                null
              ).then(
                function(){
                  var oMain = new Main();
                  return oMain.execute();
                }
              ).then(
                function () {
                  oVBox.setBusy(false);
                }
              ).catch(function (oError) {
                Log.error(oError);
                oVBox.setBusy(false);
              });
            });
          }
        ).catch(
          function (oError) {
            Log.error(oError);
          }
        );
      };
    }
    return Program;
  }
);
