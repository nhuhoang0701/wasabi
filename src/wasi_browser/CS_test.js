const endpoint = "http://lde-docker-3.dataaccess.c.eu-de-1.cloud.sap:20180/sap/cs";

const networkLayer = "JDBC";
const dbEngine = "SQLite for test - efashion";
const xmlConnectionDesc = "<?xml version=\"1.0\"?><ConnectionDefinition><Parameter Category=\"IDENTIFICATION\" ID=\"NETWORKLAYER\" Type=\"String\">JDBC</Parameter><Parameter Category=\"IDENTIFICATION\" ID=\"DBMS\" Type=\"String\">SQLite for test - memory</Parameter><Parameter Category=\"IDENTIFICATION\" ID=\"CONNECTIVITY_TYPE\" Type=\"String\">Relational</Parameter><Parameter Category=\"CONFIGURATION\" ID=\"MAX_PARALLEL_QUERIES\" Type=\"Integer\">4</Parameter><Parameter Category=\"CREDENTIALS\" ID=\"PASSWORD\" Type=\"String\"></Parameter><Parameter Category=\"CREDENTIALS\" ID=\"DATASOURCE\" Type=\"String\"></Parameter><Parameter Category=\"CREDENTIALS\" ID=\"AUTHENTICATION_MODE\" Type=\"Enum\">ConfiguredIdentity</Parameter><Parameter Category=\"CREDENTIALS\" ID=\"USER\" Type=\"String\"></Parameter></ConnectionDefinition>";

const sql = "SELECT Article_label, Sale_price FROM Article_lookup LIMIT 10;";


function _startCSTest() {
    console.log("******************************************");
    console.log("Start CS test.");
    console.log("******************************************");
    
    let cs = new ProtoCS.ConnectionServer();
    cs.configure(endpoint);
    createJob(cs).then((job) => {
        job.Execute(sql, undefined, undefined).then((result) => {
            result.setSliceSize(10);
            this.fetch(result);
        });                                
    })
}

function createJob(cs){
    console.log("******************************************");
    console.log("Creating Job.");
    console.log("******************************************");
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

async function fetch(result) {
    console.log("******************************************");
    console.log("Fetching Columns.");
    console.log("******************************************");
    for(let i = 0; i < result.Descriptions().length; i++) {
        var cd = result.Descriptions()[i];
        console.log(cd.name);
    }
    console.log("******************************************");
    console.log("Fetching Results.");
    console.log("******************************************");

    for await (let record of result.Records().asyncIterator()) {
        let line="";
        for(let i = 0; i < record.length; i++) {
            line += record[i].getValueAsString()+"\t";
        }
        console.log(line);
    }
    console.log("All fetched!");
}