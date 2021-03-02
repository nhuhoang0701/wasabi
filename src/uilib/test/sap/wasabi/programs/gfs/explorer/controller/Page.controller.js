/*!
 * ${copyright}
 */
/*global programs, sap*/
sap.ui.define(
  [
    "sap/base/Log",
    "sap/ui/core/mvc/Controller",
    "sap/firefly/fiori/thirdparty/lodash"
  ],
  function (
    Log,
    Controller,
    _
  ) {
    "use strict";
    function listFiles(oModel) {
      return window.gapi.client.drive.files.list(
        {
          "pageSize": 500,
//          q: "mimeType='application/vnd.google-apps.folder'",
          "fields": "nextPageToken, files(id, name, kind, mimeType, webViewLink, parents)"
        }
      ).then(
        function(response) {
          Log.info("Files:");
          var files = response.result.files;
          var aRoot = _.filter(
            files,function( o){
              if(!o.parents){
                return true;
              }
              var s = o.parents[0];
              return ! _.some(
                files,
                function(o){
                  return o.id === s;
                });
            }
          );
          function ajouter( oNode, aNodes){
            return _.reduce(
              _.filter(
                aNodes,
                function( o ){
                  return o.parents && o.parents[0] === oNode.id;
                }
              ),
              function( oC, o ){
                if(!oC.items){
                  oC.items = [];
                }
                oC.items.push( o);
                ajouter( o, aNodes );
                return oC;
              }, oNode
            );
          }
          oModel.setData(
            {
              items: _.map(
                aRoot, function(o){
                  return ajouter( o, files);
                }
              )
            }
          );
        }
      );
    }
    Controller.extend(
      "programs.gfs.explorer.controller.Page", {
        onInit: function () {
          var that = this;
          that.getView().setBusy(true);
          listFiles( that.getView().getModel()).then(
            function(){
              that.getView().setBusy(false);
            }
          ).catch(
            function(oError){
              Log.error(oError);
              that.getView().setBusy(false);
            }
          );
        },
        copy2vfs: function(oEvent){
          var that = this;
          var aItems = that.getView().byId("Tree").getSelectedItems();
          _.map( aItems, function(oItem){
            console.log(oItem.getTitle());
          });
        },
        onPress: function(oEvent){
          Log.info(oEvent.getParameter("listItem").data("ln"));
          window.open(oEvent.getParameter("listItem").data("ln"));
        }
      }
    );
    return programs.gfs.explorer.controller.Page;
  }
);
