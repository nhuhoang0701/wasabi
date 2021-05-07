const endpoint = "http://lde-docker-3.dataaccess.c.eu-de-1.cloud.sap:20180/sap/cs";

const networkLayer = "JDBC";
const dbEngine = "SQLite for test - efashion";
const xmlConnectionDesc = "<?xml version=\"1.0\"?><ConnectionDefinition><Parameter Category=\"IDENTIFICATION\" ID=\"NETWORKLAYER\" Type=\"String\">JDBC</Parameter><Parameter Category=\"IDENTIFICATION\" ID=\"DBMS\" Type=\"String\">SQLite for test - memory</Parameter><Parameter Category=\"IDENTIFICATION\" ID=\"CONNECTIVITY_TYPE\" Type=\"String\">Relational</Parameter><Parameter Category=\"CONFIGURATION\" ID=\"MAX_PARALLEL_QUERIES\" Type=\"Integer\">4</Parameter><Parameter Category=\"CREDENTIALS\" ID=\"PASSWORD\" Type=\"String\"></Parameter><Parameter Category=\"CREDENTIALS\" ID=\"DATASOURCE\" Type=\"String\"></Parameter><Parameter Category=\"CREDENTIALS\" ID=\"AUTHENTICATION_MODE\" Type=\"Enum\">ConfiguredIdentity</Parameter><Parameter Category=\"CREDENTIALS\" ID=\"USER\" Type=\"String\"></Parameter></ConnectionDefinition>";

function cs_log(msg) {
    if(typeof document !== 'undefined') {
        let msgCR = msg + '\n';
        document.getElementById('log').innerHTML += msgCR.replace(/\n( *)/g, function (match, p1) {
            return '<br>' + '&nbsp;'.repeat(p1.length);
        });
    }
    console.log(msg);
}

function _startCSTest() {
    cs_log("******************************************");
    cs_log("Start CS test.");
    cs_log("******************************************");
    
    let cs = new ProtoCS.ConnectionServer();
    cs.configure(endpoint);
    cs_createJob(cs).then((job) => {
        let sql = document.querySelector('#enter').value;
        cs_log("Execute SQL:" + sql);
        job.Execute(sql, undefined, undefined).then((result) => {
            result.setSliceSize(500);
            this.cs_fetch(result);
        }).catch(error=>{
            cs_log("ERROR: id:" + error.id);
            cs_log("       type:" + error.type);
            cs_log("       message:" + error.message);
        });                            
    })
}

function cs_createJob(cs){
    cs_log("******************************************");
    cs_log("Creating Job.");
    cs_log("******************************************");

    return new Promise((resolve, reject) => {
        cs.JobProfile(networkLayer, dbEngine).then((jd) => {
            jd.Properties().then((jps) => {
                cs.CreateConnectionManager(networkLayer, dbEngine).then((connMgr) => {
                    connMgr.BuildBOStrings(xmlConnectionDesc).then(([connectString, dummy]) => {
                        cs.CreateJob(connectString, networkLayer, dbEngine, jps).then((job) => {
                            resolve(job);
                        });
                    });
                });
            });
        });
    })
}

async function cs_fetch(result) {
    cs_log("******************************************");
    cs_log("Fetching Columns.");
    cs_log("******************************************");
    for(let i = 0; i < result.Descriptions().length; i++) {
        var cd = result.Descriptions()[i];
        cs_log(cd.name + " " + cd.typeName);
    }
    cs_log("******************************************");
    cs_log("Fetching Results.");
    cs_log("******************************************");

    for await (let record of result.Records().asyncIterator()) {
        let line="";
        for(let i = 0; i < record.length; i++) {
            line += record[i].getValueAsString()+"\t";
        }
        cs_log(line);
    }
    cs_log("All fetched!");
}