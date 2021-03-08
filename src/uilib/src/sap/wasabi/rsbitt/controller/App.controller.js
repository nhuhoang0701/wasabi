/*
 * ${copyright}
 */
/*global sap*/
sap.ui.define(
  "sap/wasabi/rsbitt/controller/App.controller",
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
          oComp.getMain().getServerInfo().then(
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
          oComp.getMain().getResponse(
            that.getView().getModel().getProperty("/requestBody")
          ).then(
            function(s){
              that.getView().getModel().setProperty("/responseBody",s);
              this.getView().byId("editorResponse").prettyPrint();
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
        prettyPrint: function(){
          this.getView().byId("editorRequest").prettyPrint();
          this.getView().byId("editorResponse").prettyPrint();
        }
      }
    );
    return sap.wasabi.rsbitt.controller.App;
  }
);
