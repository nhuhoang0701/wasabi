/*
 * ${copyright}
 */
/*global sap*/
sap.ui.define(
  "sap/wasbi/rsbitt/controller/App.controller",
  [
    "jquery.sap.global",
    "sap/base/Log",
    "sap/ui/core/mvc/Controller"
  ],
  /* eslint-disable max-params */
  function (
    jQuery,
    Log,
    Controller
  )
  {
    "use strict";
    Controller.extend(
      "sap.wasabi.rsbitt.controller.App", {
        serverInfo: function(){
          var that = this;
          var oComp =that.getOwnerComponent();
          that.getView().setBusy(true);
          oComp.getMain().executeServerInfo().then(
            function(s){
              that.getView().getModel().setProperty("/responseBody",s);
              that.getView().setBusy(false);
            }
          ).catch(
            function(oError){
              Log.error(oError);
              that.getView().getModel().setProperty(
                "/responseBody",
                oError.stack
              );
              that.getView().setBusy(false);
            }
          );
        },
        getResponse: function(){
          var that = this;
          var oComp =that.getOwnerComponent();
          that.getView().setBusy(true);
          oComp.getMain().executeGetResponse(
            that.getView().getModel().getProperty("/requestBody")
          ).then(
            function(s){
              that.getView().getModel().setProperty("/responseBody",s);
              that.getView().setBusy(false);
            }
          ).catch(
            function(oError){
              Log.error(oError);
              that.getView().getModel().setProperty(
                "/responseBody",
                oError.stack
              );
              that.getView().setBusy(false);
            }
          );
        }
      }
    );
    return sap.wasabi.rsbitt.controller.App;
  }
);
