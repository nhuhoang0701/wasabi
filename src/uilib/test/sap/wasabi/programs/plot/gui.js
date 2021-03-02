/**
 * 
 */
sap.ui.define([
        "sap/base/Log",
        "programs/plot/gnuplot_api"
    ],
    function (Log, gnuplot) {
        "use strict";

        function GUI(dataCM, plotCM, oImage) {
            gnuplot.text = "";
            gnuplot.files = [];

            function updateListing() {
                return gnuplot.getListing().then(
                    function (l) {
                        for (var nameidx in l) {
                            var name = l[nameidx];
                            if (name.search(".png") > 0)
                                gnuplot.getFile(name).then(
                                    function (oContent) {

                                        toGnuplotImage(oContent, "image/png");
                                    });
                            if (name.search(".jpg") > 0 || name.search(".jpeg") > 0)
                                gnuplot.getFile(name).then(function (oContent) {

                                    toGnuplotImage(oContent, "image/jpeg");
                                });
                            if (name.search(".gif") > 0)
                                gnuplot.getFile(name).then(function (oContent) {

                                    toGnuplotImage(oContent, "image/gif");
                                });
                            if (name.search(".svg") > 0)
                                gnuplot.getFile(name).then(function (oContent) {
                                    toGnuplotImage(oContent, "image/svg+xml");
                                });
                        }

                    });
            }

            function toGnuplotImage(data, mimestring) {
                try {
                    var ab = new Uint8Array(data);

                    var rstr = "";
                    for (var i = 0; i < data.length; i++) {
                        rstr += String.fromCharCode(data[i]);
                    }
                    window._mimestring = rstr;
                    var blob = new Blob([ab], {
                        "type": mimestring
                    });
                    window.URL = window.URL || window.webkitURL;
                    var urlBlob = window.URL.createObjectURL(blob);
                    oImage.setSrc(urlBlob);
                    return urlBlob;
                } catch (err) { // in case blob / URL missing, fallback to data-uri
                }
            };

            function run() {
                // "upload" files to worker thread
                for (var f in gnuplot.files)
                    gnuplot.putFile(f, gnuplot.files[f]);
                gnuplot.run(["script.txt"], function (e) {
                    updateListing(true);
                });
            };
            return gnuplot.putFile("data.txt", dataCM.getValue()).then(
                function () {
                    return gnuplot.putFile("script.txt", plotCM.getValue());
                }).then(
                function () {
                    sap.firefly.fiori.Log.error("run");
                    return run();
                }).then(
                function () {
                    sap.firefly.fiori.Log.error("listing");
                    return updateListing();
                }).catch(
                function (oError) {
                    Log.error(oError);
                });
        }
        return GUI;
    });