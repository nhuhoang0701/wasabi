/*
 * ${copyright}
 */
/*global sap*/
sap.ui.define(
  [
    "sap/base/Log",
    "programs/plot/gui",
  ],
  function (Log, gui) {
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
        return Module.ff_openWindowPromise("GnuPlot").then(
          function (oDE) {
            var oVBox = new sap.m.TabContainer();
            oVBox.placeAt(oDE.parentNode, 0);
            jQuery(oDE).remove();
            oItem = new sap.m.TabContainerItem({
              name: "Data"
            });
            oVBox.addItem(oItem);
            var oTBData = new sap.ui.codeeditor.CodeEditor({
              height: "300px",
              type: "text",
              width: "100%"

            });
            oItem.addContent(oTBData);
            oItem = new sap.m.TabContainerItem({
              name: "Script"
            });
            oVBox.addItem(oItem);
            var oTBPlot = new sap.ui.codeeditor.CodeEditor({
              height: "300px",
              type: "text",
              width: "100%"

            });
            oItem.addContent(oTBPlot);
            oVBox.addItem(oItem);


            var oItem = new sap.m.TabContainerItem({
              name: "Image"
            });
            oVBox.addItem(oItem);
            var oButton = new sap.m.Button({
              text: "Save"
            });
            oButton.attachPress(function () {
              var sName = ["/", "PlotImage(" + getCount() + ")", ".svg"].join("");
              Module.writeVfsFile(sName, window._mimestring || "empty");
            });
            oItem.addContent(oButton);
            var oImg = new sap.m.Image();
            oItem.addContent(oImg);
            oVBox.attachItemSelect(function () {

              oVBox.setBusy(true);
              gui(oTBData, oTBPlot, oImg).then(
                function () {
                  oVBox.setBusy(false);
                }
              ).catch(function (oError) {
                sap.firefly.fiori.Log.error(oError);
                oVBox.setBusy(false);
              });
            });
            Promise.resolve(
              $.ajax({
                url: "programs/plot/default_data.txt"
              })).then(
                function (d) {
                  sap.firefly.fiori.Log.error("data.txt");
                  oTBData.setValue(d);
                  return Promise.resolve(
                    $.ajax({
                      url: "programs/plot/default_script.txt"
                    })
                  );
                }).then(
                  function (d) {
                    oTBPlot.setValue(d);
                    return gui(oTBData, oTBPlot, oImg);
                  });
          }).catch(function (oError) {
            Log.error(oError);
          });
      }

    }
    return Program;
  }
)
