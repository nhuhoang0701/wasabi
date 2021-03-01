sap.ui.define(
    [],
    function () {
        [
            "highstock"
            /*
              ,"grouped-categories",
              "highcharts-3d",
              "highcharts-more",
              */

        ].forEach(function (s) {
            jQuery.sap.require(
                ["sap.firefly.fiori.thirdparty.hc", s].join(".")
            );
        });
        return Highcharts;
    }
);