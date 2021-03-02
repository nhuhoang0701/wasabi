/*global sap*/
sap.ui.define(
  [
    "sap/base/Log",
    "sap/ui/core/UIComponent"
  ],
  function(Log,UIComponent){
    var GFS =  UIComponent.extend(
      "programs.gfs.explorer.Component", {
        metadata: {
          manifest: "json"
        }
      });
    return GFS;
  }
);
