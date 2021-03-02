/*!
 * ${copyright}
 */
/*global sap */
sap.ui.define(
  "sap/firefly/fiori/rsrt/Component",
  [
    "sap/ui/core/UIComponent",
    "sap/base/Log",
    "sap/wasabi/main",
    "sap/wasabi/thirdparty/lodash"
  ],
  function(
    UIComponent, Log, Main, _
  ) {
    "use strict";
    /**
     * RSBITT Test Application for the InA based WASABi processor.
     *
     * This <code>UIComponent</code> allows for testing navigation in multidimensional data
     * provided by a server implementing the the <a href="https://wiki.scn.sap.com/wiki/display/BI/OT-BICS-INA" target="_blank">InA Protocol</a>. 
     * It roughly corresponds to the familiar transaction <a href="https://wiki.scn.sap.com/wiki/display/BI/Transaction+RSRT%3A+Query+Monitor" target="_blank">RSRT</a>. 
     * @extends sap.ui.core.UIComponent
     * @class
     * @author SAP SE
     * @version 1.88.0-SNAPSHOT
     * @public
     * @experimental
     * @alias sap.wasabi.rsbitt.Component
     */
    var RSBITT = UIComponent.extend(
      "sap.wasabi.rsbitt.Component", {
        metadata: {
          manifest: "json"
        },
        init: function () {
          var that = this;
          UIComponent.prototype.init.apply(that, arguments);
          Log.info("Load RSBITT");
          that.getMain = _.constant(new Main());
        },
        exit: function () {
          var that = this;
          var oView = that.getRootControl();
          oView.destroyDependents();
          oView.destroyContent();
          oView.removeAllDependents();
          oView.destroy();
        }
      });
    return RSBITT;
  }
);
