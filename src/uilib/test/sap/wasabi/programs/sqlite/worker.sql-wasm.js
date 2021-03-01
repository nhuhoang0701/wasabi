sap.ui.define(
        [
            "sap/base/Log",
            "sap/firefly/fiori/thirdparty/lodash"
        ],
        function (Log, _) {
            Log.info("Test");

            function WasmRunner(Module) {
                var that = this;
                that.getModule = _.constant(Module);
                var initSqlJsPromise = undefined;

                var initSqlJs = function (moduleConfig) {
                        var Module = that.getModule();

                        initSqlJsPromise = new Promise(function (resolveModule, reject) {
                                    var Module = that.getModule();
                                    // We are modularizing this manually because the current modularize setting in Emscripten has some issues:
                                    // https://github.com/kripken/emscripten/issues/5820

                                    // The way to affect the loading of emcc compiled modules is to create a variable called `Module` and add
                                    // properties to it, like `preRun`, `postRun`, etc
                                    // We are using that to get notified when the WASM has finished loading.
                                    // Only then will we return our promise

                                    // If they passed in a moduleConfig object, use that
                                    // Otherwise, initialize Module to the empty object

                                    // EMCC only allows for a single onAbort function (not an array of functions)
                                    // So if the user defined their own onAbort function, we remember it and call it
                                    var originalOnAbortFunction = Module['onAbort'];
                                    Module['onAbort'] = function (errorThatCausedAbort) {
                                        reject(new Error(errorThatCausedAbort));
                                        if (originalOnAbortFunction) {
                                            originalOnAbortFunction(errorThatCausedAbort);
                                        }
                                    };

                                    Module['postRun'] = Module['postRun'] || [];
                                    Module['postRun'].push(function () {
                                        // When Emscripted calls postRun, this promise resolves with the built Module
                                        resolveModule(Module);
                                    });

                                    // There is a section of code in the emcc-generated code below that looks like this:
                                    // (Note that this is lowercase `module`)
                                    // if (typeof module !== 'undefined') {
                                    //     module['exports'] = Module;
                                    // }
                                    // When that runs, it's going to overwrite our own modularization export efforts in shell-post.js!
                                    // The only way to tell emcc not to emit it is to pass the MODULARIZE=1 or MODULARIZE_INSTANCE=1 flags,
                                    // but that carries with it additional unnecessary baggage/bugs we don't want either.
                                    // So, we have three options:
                                    // 1) We undefine `module`
                                    // 2) We remember what `module['exports']` was at the beginning of this function and we restore it later
                                    // 3) We write a script to remove those lines of code as part of the Make process.
                                    //
                                    // Since those are the only lines of code that care about module, we will undefine it. It's the most straightforward
                                    // of the options, and has the side effect of reducing emcc's efforts to modify the module if its output were to change in the future.
                                    // That's a nice side effect since we're handling the modularization efforts ourselves
                                    module = undefined;

                                    // The emcc-generated code and shell-post.js code goes below,
                                    // meaning that all of it runs inside of this promise. If anything throws an exception, our promise will abort
var e;e||(e=typeof Module !== 'undefined' ? Module : {});null;
e.onRuntimeInitialized=function(){function a(h,l){this.Pa=h;this.db=l;this.Oa=1;this.gb=[]}function b(h){this.filename="dbfile_"+(4294967295*Math.random()>>>0);if(null!=h){var l=this.filename,q=l?k("//"+l):"/";l=aa(!0,!0);q=ba(q,(void 0!==l?l:438)&4095|32768,0);if(h){if("string"===typeof h){for(var v=Array(h.length),B=0,R=h.length;B<R;++B)v[B]=h.charCodeAt(B);h=v}ca(q,l|146);v=m(q,"w");da(v,h,0,h.length,0,void 0);ea(v);ca(q,l)}}this.handleError(f(this.filename,c));this.db=p(c,"i32");qc(this.db);this.eb=
{};this.Va={}}var c=r(4),d=e.cwrap,f=d("sqlite3_open","number",["string","number"]),g=d("sqlite3_close_v2","number",["number"]),n=d("sqlite3_exec","number",["number","string","number","number","number"]),t=d("sqlite3_changes","number",["number"]),w=d("sqlite3_prepare_v2","number",["number","string","number","number","number"]),u=d("sqlite3_prepare_v2","number",["number","number","number","number","number"]),C=d("sqlite3_bind_text","number",["number","number","number","number","number"]),H=d("sqlite3_bind_blob",
"number",["number","number","number","number","number"]),ha=d("sqlite3_bind_double","number",["number","number","number"]),rc=d("sqlite3_bind_int","number",["number","number","number"]),sc=d("sqlite3_bind_parameter_index","number",["number","string"]),tc=d("sqlite3_step","number",["number"]),uc=d("sqlite3_errmsg","string",["number"]),vc=d("sqlite3_column_count","number",["number"]),wc=d("sqlite3_data_count","number",["number"]),xc=d("sqlite3_column_double","number",["number","number"]),yc=d("sqlite3_column_text",
"string",["number","number"]),zc=d("sqlite3_column_blob","number",["number","number"]),Ac=d("sqlite3_column_bytes","number",["number","number"]),Bc=d("sqlite3_column_type","number",["number","number"]),Cc=d("sqlite3_column_name","string",["number","number"]),Dc=d("sqlite3_reset","number",["number"]),Ec=d("sqlite3_clear_bindings","number",["number"]),Fc=d("sqlite3_finalize","number",["number"]),Gc=d("sqlite3_create_function_v2","number","number string number number number number number number number".split(" ")),
Hc=d("sqlite3_value_type","number",["number"]),Ic=d("sqlite3_value_bytes","number",["number"]),Jc=d("sqlite3_value_text","string",["number"]),Kc=d("sqlite3_value_blob","number",["number"]),Lc=d("sqlite3_value_double","number",["number"]),Mc=d("sqlite3_result_double","",["number","number"]),tb=d("sqlite3_result_null","",["number"]),Nc=d("sqlite3_result_text","",["number","string","number","number"]),Oc=d("sqlite3_result_blob","",["number","number","number","number"]),Pc=d("sqlite3_result_int","",["number",
"number"]),ub=d("sqlite3_result_error","",["number","string","number"]),qc=d("RegisterExtensionFunctions","number",["number"]);a.prototype.bind=function(h){if(!this.Pa)throw"Statement closed";this.reset();return Array.isArray(h)?this.ub(h):null!=h&&"object"===typeof h?this.vb(h):!0};a.prototype.step=function(){if(!this.Pa)throw"Statement closed";this.Oa=1;var h=tc(this.Pa);switch(h){case 100:return!0;case 101:return!1;default:throw this.db.handleError(h);}};a.prototype.Bb=function(h){null==h&&(h=
this.Oa,this.Oa+=1);return xc(this.Pa,h)};a.prototype.Cb=function(h){null==h&&(h=this.Oa,this.Oa+=1);return yc(this.Pa,h)};a.prototype.getBlob=function(h){null==h&&(h=this.Oa,this.Oa+=1);var l=Ac(this.Pa,h);var q=zc(this.Pa,h);var v=new Uint8Array(l);for(h=0;h<l;)v[h]=x[q+h],h+=1;return v};a.prototype.get=function(h){var l;null!=h&&this.bind(h)&&this.step();var q=[];h=0;for(l=wc(this.Pa);h<l;){switch(Bc(this.Pa,h)){case 1:case 2:q.push(this.Bb(h));break;case 3:q.push(this.Cb(h));break;case 4:q.push(this.getBlob(h));
break;default:q.push(null)}h+=1}return q};a.prototype.getColumnNames=function(){var h;var l=[];var q=0;for(h=vc(this.Pa);q<h;)l.push(Cc(this.Pa,q)),q+=1;return l};a.prototype.getAsObject=function(h){var l;var q=this.get(h);var v=this.getColumnNames();var B={};h=0;for(l=v.length;h<l;){var R=v[h];B[R]=q[h];h+=1}return B};a.prototype.run=function(h){null!=h&&this.bind(h);this.step();return this.reset()};a.prototype.yb=function(h,l){null==l&&(l=this.Oa,this.Oa+=1);h=fa(h);var q=ia(h);this.gb.push(q);
this.db.handleError(C(this.Pa,l,q,h.length-1,0))};a.prototype.tb=function(h,l){null==l&&(l=this.Oa,this.Oa+=1);var q=ia(h);this.gb.push(q);this.db.handleError(H(this.Pa,l,q,h.length,0))};a.prototype.xb=function(h,l){null==l&&(l=this.Oa,this.Oa+=1);this.db.handleError((h===(h|0)?rc:ha)(this.Pa,l,h))};a.prototype.wb=function(h){null==h&&(h=this.Oa,this.Oa+=1);H(this.Pa,h,0,0,0)};a.prototype.mb=function(h,l){null==l&&(l=this.Oa,this.Oa+=1);switch(typeof h){case "string":this.yb(h,l);return;case "number":case "boolean":this.xb(h+
0,l);return;case "object":if(null===h){this.wb(l);return}if(null!=h.length){this.tb(h,l);return}}throw"Wrong API use : tried to bind a value of an unknown type ("+h+").";};a.prototype.vb=function(h){var l=this;Object.keys(h).forEach(function(q){var v=sc(l.Pa,q);0!==v&&l.mb(h[q],v)});return!0};a.prototype.ub=function(h){var l;for(l=0;l<h.length;)this.mb(h[l],l+1),l+=1;return!0};a.prototype.reset=function(){return 0===Ec(this.Pa)&&0===Dc(this.Pa)};a.prototype.freemem=function(){for(var h;void 0!==(h=
this.gb.pop());)ja(h)};a.prototype.free=function(){var h=0===Fc(this.Pa);delete this.db.eb[this.Pa];this.Pa=0;return h};b.prototype.run=function(h,l){if(!this.db)throw"Database closed";if(l){h=this.prepare(h,l);try{h.step()}finally{h.free()}}else this.handleError(n(this.db,h,0,0,c));return this};b.prototype.exec=function(h,l){if(!this.db)throw"Database closed";var q=ka();try{var v=la(h)+1,B=r(v);ma(h,x,B,v);var R=B;var E=r(4);for(h=[];0!==p(R,"i8");){na(c);na(E);this.handleError(u(this.db,R,-1,c,
E));var pa=p(c,"i32");R=p(E,"i32");if(0!==pa){var U=null;var A=new a(pa,this);for(null!=l&&A.bind(l);A.step();)null===U&&(U={columns:A.getColumnNames(),values:[]},h.push(U)),U.values.push(A.get());A.free()}}return h}catch(L){throw A&&A.free(),L;}finally{oa(q)}};b.prototype.each=function(h,l,q,v){"function"===typeof l&&(v=q,q=l,l=void 0);h=this.prepare(h,l);try{for(;h.step();)q(h.getAsObject())}finally{h.free()}if("function"===typeof v)return v()};b.prototype.prepare=function(h,l){na(c);this.handleError(w(this.db,
h,-1,c,0));h=p(c,"i32");if(0===h)throw"Nothing to prepare";var q=new a(h,this);null!=l&&q.bind(l);return this.eb[h]=q};b.prototype["export"]=function(){Object.values(this.eb).forEach(function(l){l.free()});Object.values(this.Va).forEach(qa);this.Va={};this.handleError(g(this.db));var h=ra(this.filename);this.handleError(f(this.filename,c));this.db=p(c,"i32");return h};b.prototype.close=function(){null!==this.db&&(Object.values(this.eb).forEach(function(h){h.free()}),Object.values(this.Va).forEach(qa),
this.Va={},this.handleError(g(this.db)),sa("/"+this.filename),this.db=null)};b.prototype.handleError=function(h){if(0===h)return null;h=uc(this.db);throw Error(h);};b.prototype.getRowsModified=function(){return t(this.db)};b.prototype.create_function=function(h,l){Object.prototype.hasOwnProperty.call(this.Va,h)&&(ta(this.Va[h]),delete this.Va[h]);var q=ua(function(v,B,R){for(var E,pa=[],U=0;U<B;U+=1){var A=p(R+4*U,"i32"),L=Hc(A);if(1===L||2===L)A=Lc(A);else if(3===L)A=Jc(A);else if(4===L){L=A;A=Ic(L);
L=Kc(L);for(var zb=new Uint8Array(A),Ca=0;Ca<A;Ca+=1)zb[Ca]=x[L+Ca];A=zb}else A=null;pa.push(A)}try{E=l.apply(null,pa)}catch(Sc){ub(v,Sc,-1);return}switch(typeof E){case "boolean":Pc(v,E?1:0);break;case "number":Mc(v,E);break;case "string":Nc(v,E,-1,-1);break;case "object":null===E?tb(v):null!=E.length?(B=ia(E),Oc(v,B,E.length,-1),ja(B)):ub(v,"Wrong API use : tried to return a value of an unknown type ("+E+").",-1);break;default:tb(v)}});this.Va[h]=q;this.handleError(Gc(this.db,h,l.length,1,0,q,0,
0,0));return this};e.Database=b};var va={},y;for(y in e)e.hasOwnProperty(y)&&(va[y]=e[y]);var wa="./this.program",xa=!1,ya=!1,za=!1,Aa=!1;xa="object"===typeof window;ya="function"===typeof importScripts;za="object"===typeof process&&"object"===typeof process.versions&&"string"===typeof process.versions.node;Aa=!xa&&!za&&!ya;var z="",Ba,Da,Ea,Fa;
if(za)z=ya?require("path").dirname(z)+"/":__dirname+"/",Ba=function(a,b){Ea||(Ea=require("fs"));Fa||(Fa=require("path"));a=Fa.normalize(a);return Ea.readFileSync(a,b?null:"utf8")},Da=function(a){a=Ba(a,!0);a.buffer||(a=new Uint8Array(a));assert(a.buffer);return a},1<process.argv.length&&(wa=process.argv[1].replace(/\\/g,"/")),process.argv.slice(2),"undefined"!==typeof module&&(module.exports=e),process.on("unhandledRejection",D),e.inspect=function(){return"[Emscripten Module object]"};else if(Aa)"undefined"!=
typeof read&&(Ba=function(a){return read(a)}),Da=function(a){if("function"===typeof readbuffer)return new Uint8Array(readbuffer(a));a=read(a,"binary");assert("object"===typeof a);return a},"undefined"!==typeof print&&("undefined"===typeof console&&(console={}),console.log=print,console.warn=console.error="undefined"!==typeof printErr?printErr:print);else if(xa||ya)ya?z=self.location.href:document.currentScript&&(z=document.currentScript.src),z=0!==z.indexOf("blob:")?z.substr(0,z.lastIndexOf("/")+
1):"",Ba=function(a){var b=new XMLHttpRequest;b.open("GET",a,!1);b.send(null);return b.responseText},ya&&(Da=function(a){var b=new XMLHttpRequest;b.open("GET",a,!1);b.responseType="arraybuffer";b.send(null);return new Uint8Array(b.response)});var Ga=e.print||console.log.bind(console),F=e.printErr||console.warn.bind(console);for(y in va)va.hasOwnProperty(y)&&(e[y]=va[y]);va=null;e.thisProgram&&(wa=e.thisProgram);var Ha=[],Ia;function ta(a){Ia.delete(G.get(a));Ha.push(a)}
function ua(a){if(!Ia){Ia=new WeakMap;for(var b=0;b<G.length;b++){var c=G.get(b);c&&Ia.set(c,b)}}if(Ia.has(a))a=Ia.get(a);else{if(Ha.length)b=Ha.pop();else{b=G.length;try{G.grow(1)}catch(g){if(!(g instanceof RangeError))throw g;throw"Unable to grow wasm table. Set ALLOW_TABLE_GROWTH.";}}try{G.set(b,a)}catch(g){if(!(g instanceof TypeError))throw g;if("function"===typeof WebAssembly.Function){var d={i:"i32",j:"i64",f:"f32",d:"f64"},f={parameters:[],results:[]};for(c=1;4>c;++c)f.parameters.push(d["viii"[c]]);
c=new WebAssembly.Function(f,a)}else{d=[1,0,1,96];f={i:127,j:126,f:125,d:124};d.push(3);for(c=0;3>c;++c)d.push(f["iii"[c]]);d.push(0);d[1]=d.length-2;c=new Uint8Array([0,97,115,109,1,0,0,0].concat(d,[2,7,1,1,101,1,102,0,0,7,5,1,1,102,0,0]));c=new WebAssembly.Module(c);c=(new WebAssembly.Instance(c,{e:{f:a}})).exports.f}G.set(b,c)}Ia.set(a,b);a=b}return a}function qa(a){ta(a)}var Ja;e.wasmBinary&&(Ja=e.wasmBinary);var noExitRuntime;e.noExitRuntime&&(noExitRuntime=e.noExitRuntime);
"object"!==typeof WebAssembly&&D("no native wasm support detected");
function na(a){var b="i32";"*"===b.charAt(b.length-1)&&(b="i32");switch(b){case "i1":x[a>>0]=0;break;case "i8":x[a>>0]=0;break;case "i16":Ka[a>>1]=0;break;case "i32":I[a>>2]=0;break;case "i64":J=[0,(K=0,1<=+La(K)?0<K?(Ma(+Na(K/4294967296),4294967295)|0)>>>0:~~+Oa((K-+(~~K>>>0))/4294967296)>>>0:0)];I[a>>2]=J[0];I[a+4>>2]=J[1];break;case "float":Pa[a>>2]=0;break;case "double":Qa[a>>3]=0;break;default:D("invalid type for setValue: "+b)}}
function p(a,b){b=b||"i8";"*"===b.charAt(b.length-1)&&(b="i32");switch(b){case "i1":return x[a>>0];case "i8":return x[a>>0];case "i16":return Ka[a>>1];case "i32":return I[a>>2];case "i64":return I[a>>2];case "float":return Pa[a>>2];case "double":return Qa[a>>3];default:D("invalid type for getValue: "+b)}return null}var Ra,G=new WebAssembly.Table({initial:387,element:"anyfunc"}),Sa=!1;function assert(a,b){a||D("Assertion failed: "+b)}
function Ta(a){var b=e["_"+a];assert(b,"Cannot call unknown function "+a+", make sure it is exported");return b}
function Ua(a,b,c,d){var f={string:function(u){var C=0;if(null!==u&&void 0!==u&&0!==u){var H=(u.length<<2)+1;C=r(H);ma(u,M,C,H)}return C},array:function(u){var C=r(u.length);x.set(u,C);return C}},g=Ta(a),n=[];a=0;if(d)for(var t=0;t<d.length;t++){var w=f[c[t]];w?(0===a&&(a=ka()),n[t]=w(d[t])):n[t]=d[t]}c=g.apply(null,n);c=function(u){return"string"===b?N(u):"boolean"===b?!!u:u}(c);0!==a&&oa(a);return c}var Va=0,Wa=2;
function ia(a){var b=Va;if("number"===typeof a){var c=!0;var d=a}else c=!1,d=a.length;var f;b==Wa?f=g:f=[Xa,r][b](Math.max(d,1));if(c){var g=f;assert(0==(f&3));for(a=f+(d&-4);g<a;g+=4)I[g>>2]=0;for(a=f+d;g<a;)x[g++>>0]=0;return f}a.subarray||a.slice?M.set(a,f):M.set(new Uint8Array(a),f);return f}var Ya="undefined"!==typeof TextDecoder?new TextDecoder("utf8"):void 0;
function Za(a,b,c){var d=b+c;for(c=b;a[c]&&!(c>=d);)++c;if(16<c-b&&a.subarray&&Ya)return Ya.decode(a.subarray(b,c));for(d="";b<c;){var f=a[b++];if(f&128){var g=a[b++]&63;if(192==(f&224))d+=String.fromCharCode((f&31)<<6|g);else{var n=a[b++]&63;f=224==(f&240)?(f&15)<<12|g<<6|n:(f&7)<<18|g<<12|n<<6|a[b++]&63;65536>f?d+=String.fromCharCode(f):(f-=65536,d+=String.fromCharCode(55296|f>>10,56320|f&1023))}}else d+=String.fromCharCode(f)}return d}function N(a){return a?Za(M,a,void 0):""}
function ma(a,b,c,d){if(!(0<d))return 0;var f=c;d=c+d-1;for(var g=0;g<a.length;++g){var n=a.charCodeAt(g);if(55296<=n&&57343>=n){var t=a.charCodeAt(++g);n=65536+((n&1023)<<10)|t&1023}if(127>=n){if(c>=d)break;b[c++]=n}else{if(2047>=n){if(c+1>=d)break;b[c++]=192|n>>6}else{if(65535>=n){if(c+2>=d)break;b[c++]=224|n>>12}else{if(c+3>=d)break;b[c++]=240|n>>18;b[c++]=128|n>>12&63}b[c++]=128|n>>6&63}b[c++]=128|n&63}}b[c]=0;return c-f}
function la(a){for(var b=0,c=0;c<a.length;++c){var d=a.charCodeAt(c);55296<=d&&57343>=d&&(d=65536+((d&1023)<<10)|a.charCodeAt(++c)&1023);127>=d?++b:b=2047>=d?b+2:65535>=d?b+3:b+4}return b}function $a(a){var b=la(a)+1,c=Xa(b);c&&ma(a,x,c,b);return c}var ab,x,M,Ka,I,Pa,Qa;
function bb(a){ab=a;e.HEAP8=x=new Int8Array(a);e.HEAP16=Ka=new Int16Array(a);e.HEAP32=I=new Int32Array(a);e.HEAPU8=M=new Uint8Array(a);e.HEAPU16=new Uint16Array(a);e.HEAPU32=new Uint32Array(a);e.HEAPF32=Pa=new Float32Array(a);e.HEAPF64=Qa=new Float64Array(a)}var cb=e.INITIAL_MEMORY||16777216;e.wasmMemory?Ra=e.wasmMemory:Ra=new WebAssembly.Memory({initial:cb/65536,maximum:32768});Ra&&(ab=Ra.buffer);cb=ab.byteLength;bb(ab);var db=[],eb=[],fb=[],gb=[];
function hb(){var a=e.preRun.shift();db.unshift(a)}var La=Math.abs,Oa=Math.ceil,Na=Math.floor,Ma=Math.min,ib=0,jb=null,kb=null;e.preloadedImages={};e.preloadedAudios={};function D(a){if(e.onAbort)e.onAbort(a);F(a);Sa=!0;throw new WebAssembly.RuntimeError("abort("+a+"). Build with -s ASSERTIONS=1 for more info.");}function lb(a){var b=mb;return String.prototype.startsWith?b.startsWith(a):0===b.indexOf(a)}function nb(){return lb("data:application/octet-stream;base64,")}var mb="sql-wasm.wasm";
if(!nb()){var ob=mb;mb=e.locateFile?e.locateFile(ob,z):z+ob}function pb(){try{if(Ja)return new Uint8Array(Ja);if(Da)return Da(mb);throw"both async and sync fetching of the wasm failed";}catch(a){D(a)}}function qb(){return Ja||!xa&&!ya||"function"!==typeof fetch||lb("file://")?Promise.resolve().then(pb):fetch(mb,{credentials:"same-origin"}).then(function(a){if(!a.ok)throw"failed to load wasm binary file at '"+mb+"'";return a.arrayBuffer()}).catch(function(){return pb()})}var K,J;eb.push({Ab:function(){rb()}});
function sb(a){for(;0<a.length;){var b=a.shift();if("function"==typeof b)b(e);else{var c=b.Ab;"number"===typeof c?void 0===b.hb?G.get(c)():G.get(c)(b.hb):c(void 0===b.hb?null:b.hb)}}}function vb(a){return a.replace(/\b_Z[\w\d_]+/g,function(b){return b===b?b:b+" ["+b+"]"})}
function wb(){function a(g){return(g=g.toTimeString().match(/\(([A-Za-z ]+)\)$/))?g[1]:"GMT"}if(!xb){xb=!0;I[yb()>>2]=60*(new Date).getTimezoneOffset();var b=(new Date).getFullYear(),c=new Date(b,0,1);b=new Date(b,6,1);I[Ab()>>2]=Number(c.getTimezoneOffset()!=b.getTimezoneOffset());var d=a(c),f=a(b);d=$a(d);f=$a(f);b.getTimezoneOffset()<c.getTimezoneOffset()?(I[Bb()>>2]=d,I[Bb()+4>>2]=f):(I[Bb()>>2]=f,I[Bb()+4>>2]=d)}}var xb;
function Cb(a,b){for(var c=0,d=a.length-1;0<=d;d--){var f=a[d];"."===f?a.splice(d,1):".."===f?(a.splice(d,1),c++):c&&(a.splice(d,1),c--)}if(b)for(;c;c--)a.unshift("..");return a}function k(a){var b="/"===a.charAt(0),c="/"===a.substr(-1);(a=Cb(a.split("/").filter(function(d){return!!d}),!b).join("/"))||b||(a=".");a&&c&&(a+="/");return(b?"/":"")+a}
function Db(a){var b=/^(\/?|)([\s\S]*?)((?:\.{1,2}|[^\/]+?|)(\.[^.\/]*|))(?:[\/]*)$/.exec(a).slice(1);a=b[0];b=b[1];if(!a&&!b)return".";b&&(b=b.substr(0,b.length-1));return a+b}function Eb(a){if("/"===a)return"/";a=k(a);a=a.replace(/\/$/,"");var b=a.lastIndexOf("/");return-1===b?a:a.substr(b+1)}function Fb(a){I[Gb()>>2]=a}
function Hb(){for(var a="",b=!1,c=arguments.length-1;-1<=c&&!b;c--){b=0<=c?arguments[c]:"/";if("string"!==typeof b)throw new TypeError("Arguments to path.resolve must be strings");if(!b)return"";a=b+"/"+a;b="/"===b.charAt(0)}a=Cb(a.split("/").filter(function(d){return!!d}),!b).join("/");return(b?"/":"")+a||"."}var Ib=[];function Jb(a,b){Ib[a]={input:[],output:[],$a:b};Kb(a,Lb)}
var Lb={open:function(a){var b=Ib[a.node.rdev];if(!b)throw new O(43);a.tty=b;a.seekable=!1},close:function(a){a.tty.$a.flush(a.tty)},flush:function(a){a.tty.$a.flush(a.tty)},read:function(a,b,c,d){if(!a.tty||!a.tty.$a.qb)throw new O(60);for(var f=0,g=0;g<d;g++){try{var n=a.tty.$a.qb(a.tty)}catch(t){throw new O(29);}if(void 0===n&&0===f)throw new O(6);if(null===n||void 0===n)break;f++;b[c+g]=n}f&&(a.node.timestamp=Date.now());return f},write:function(a,b,c,d){if(!a.tty||!a.tty.$a.jb)throw new O(60);
try{for(var f=0;f<d;f++)a.tty.$a.jb(a.tty,b[c+f])}catch(g){throw new O(29);}d&&(a.node.timestamp=Date.now());return f}},Mb={qb:function(a){if(!a.input.length){var b=null;if(za){var c=Buffer.sb?Buffer.sb(256):new Buffer(256),d=0;try{d=Ea.readSync(process.stdin.fd,c,0,256,null)}catch(f){if(-1!=f.toString().indexOf("EOF"))d=0;else throw f;}0<d?b=c.slice(0,d).toString("utf-8"):b=null}else"undefined"!=typeof window&&"function"==typeof window.prompt?(b=window.prompt("Input: "),null!==b&&(b+="\n")):"function"==
typeof readline&&(b=readline(),null!==b&&(b+="\n"));if(!b)return null;a.input=fa(b,!0)}return a.input.shift()},jb:function(a,b){null===b||10===b?(Ga(Za(a.output,0)),a.output=[]):0!=b&&a.output.push(b)},flush:function(a){a.output&&0<a.output.length&&(Ga(Za(a.output,0)),a.output=[])}},Nb={jb:function(a,b){null===b||10===b?(F(Za(a.output,0)),a.output=[]):0!=b&&a.output.push(b)},flush:function(a){a.output&&0<a.output.length&&(F(Za(a.output,0)),a.output=[])}},P={Ta:null,Ua:function(){return P.createNode(null,
"/",16895,0)},createNode:function(a,b,c,d){if(24576===(c&61440)||4096===(c&61440))throw new O(63);P.Ta||(P.Ta={dir:{node:{Sa:P.La.Sa,Ra:P.La.Ra,lookup:P.La.lookup,ab:P.La.ab,rename:P.La.rename,unlink:P.La.unlink,rmdir:P.La.rmdir,readdir:P.La.readdir,symlink:P.La.symlink},stream:{Xa:P.Ma.Xa}},file:{node:{Sa:P.La.Sa,Ra:P.La.Ra},stream:{Xa:P.Ma.Xa,read:P.Ma.read,write:P.Ma.write,lb:P.Ma.lb,bb:P.Ma.bb,cb:P.Ma.cb}},link:{node:{Sa:P.La.Sa,Ra:P.La.Ra,readlink:P.La.readlink},stream:{}},nb:{node:{Sa:P.La.Sa,
Ra:P.La.Ra},stream:Ob}});c=Pb(a,b,c,d);Q(c.mode)?(c.La=P.Ta.dir.node,c.Ma=P.Ta.dir.stream,c.Ka={}):32768===(c.mode&61440)?(c.La=P.Ta.file.node,c.Ma=P.Ta.file.stream,c.Qa=0,c.Ka=null):40960===(c.mode&61440)?(c.La=P.Ta.link.node,c.Ma=P.Ta.link.stream):8192===(c.mode&61440)&&(c.La=P.Ta.nb.node,c.Ma=P.Ta.nb.stream);c.timestamp=Date.now();a&&(a.Ka[b]=c);return c},Lb:function(a){if(a.Ka&&a.Ka.subarray){for(var b=[],c=0;c<a.Qa;++c)b.push(a.Ka[c]);return b}return a.Ka},Mb:function(a){return a.Ka?a.Ka.subarray?
a.Ka.subarray(0,a.Qa):new Uint8Array(a.Ka):new Uint8Array(0)},ob:function(a,b){var c=a.Ka?a.Ka.length:0;c>=b||(b=Math.max(b,c*(1048576>c?2:1.125)>>>0),0!=c&&(b=Math.max(b,256)),c=a.Ka,a.Ka=new Uint8Array(b),0<a.Qa&&a.Ka.set(c.subarray(0,a.Qa),0))},Ib:function(a,b){if(a.Qa!=b)if(0==b)a.Ka=null,a.Qa=0;else{if(!a.Ka||a.Ka.subarray){var c=a.Ka;a.Ka=new Uint8Array(b);c&&a.Ka.set(c.subarray(0,Math.min(b,a.Qa)))}else if(a.Ka||(a.Ka=[]),a.Ka.length>b)a.Ka.length=b;else for(;a.Ka.length<b;)a.Ka.push(0);a.Qa=
b}},La:{Sa:function(a){var b={};b.dev=8192===(a.mode&61440)?a.id:1;b.ino=a.id;b.mode=a.mode;b.nlink=1;b.uid=0;b.gid=0;b.rdev=a.rdev;Q(a.mode)?b.size=4096:32768===(a.mode&61440)?b.size=a.Qa:40960===(a.mode&61440)?b.size=a.link.length:b.size=0;b.atime=new Date(a.timestamp);b.mtime=new Date(a.timestamp);b.ctime=new Date(a.timestamp);b.zb=4096;b.blocks=Math.ceil(b.size/b.zb);return b},Ra:function(a,b){void 0!==b.mode&&(a.mode=b.mode);void 0!==b.timestamp&&(a.timestamp=b.timestamp);void 0!==b.size&&P.Ib(a,
b.size)},lookup:function(){throw Qb[44];},ab:function(a,b,c,d){return P.createNode(a,b,c,d)},rename:function(a,b,c){if(Q(a.mode)){try{var d=Rb(b,c)}catch(g){}if(d)for(var f in d.Ka)throw new O(55);}delete a.parent.Ka[a.name];a.name=c;b.Ka[c]=a;a.parent=b},unlink:function(a,b){delete a.Ka[b]},rmdir:function(a,b){var c=Rb(a,b),d;for(d in c.Ka)throw new O(55);delete a.Ka[b]},readdir:function(a){var b=[".",".."],c;for(c in a.Ka)a.Ka.hasOwnProperty(c)&&b.push(c);return b},symlink:function(a,b,c){a=P.createNode(a,
b,41471,0);a.link=c;return a},readlink:function(a){if(40960!==(a.mode&61440))throw new O(28);return a.link}},Ma:{read:function(a,b,c,d,f){var g=a.node.Ka;if(f>=a.node.Qa)return 0;a=Math.min(a.node.Qa-f,d);if(8<a&&g.subarray)b.set(g.subarray(f,f+a),c);else for(d=0;d<a;d++)b[c+d]=g[f+d];return a},write:function(a,b,c,d,f,g){b.buffer===x.buffer&&(g=!1);if(!d)return 0;a=a.node;a.timestamp=Date.now();if(b.subarray&&(!a.Ka||a.Ka.subarray)){if(g)return a.Ka=b.subarray(c,c+d),a.Qa=d;if(0===a.Qa&&0===f)return a.Ka=
b.slice(c,c+d),a.Qa=d;if(f+d<=a.Qa)return a.Ka.set(b.subarray(c,c+d),f),d}P.ob(a,f+d);if(a.Ka.subarray&&b.subarray)a.Ka.set(b.subarray(c,c+d),f);else for(g=0;g<d;g++)a.Ka[f+g]=b[c+g];a.Qa=Math.max(a.Qa,f+d);return d},Xa:function(a,b,c){1===c?b+=a.position:2===c&&32768===(a.node.mode&61440)&&(b+=a.node.Qa);if(0>b)throw new O(28);return b},lb:function(a,b,c){P.ob(a.node,b+c);a.node.Qa=Math.max(a.node.Qa,b+c)},bb:function(a,b,c,d,f,g){assert(0===b);if(32768!==(a.node.mode&61440))throw new O(43);a=a.node.Ka;
if(g&2||a.buffer!==ab){if(0<d||d+c<a.length)a.subarray?a=a.subarray(d,d+c):a=Array.prototype.slice.call(a,d,d+c);d=!0;g=16384*Math.ceil(c/16384);for(b=Xa(g);c<g;)x[b+c++]=0;c=b;if(!c)throw new O(48);x.set(a,c)}else d=!1,c=a.byteOffset;return{Hb:c,fb:d}},cb:function(a,b,c,d,f){if(32768!==(a.node.mode&61440))throw new O(43);if(f&2)return 0;P.Ma.write(a,b,0,d,c,!1);return 0}}},Sb=null,Tb={},S=[],Ub=1,T=null,Vb=!0,V={},O=null,Qb={};
function W(a,b){a=Hb("/",a);b=b||{};if(!a)return{path:"",node:null};var c={pb:!0,kb:0},d;for(d in c)void 0===b[d]&&(b[d]=c[d]);if(8<b.kb)throw new O(32);a=Cb(a.split("/").filter(function(n){return!!n}),!1);var f=Sb;c="/";for(d=0;d<a.length;d++){var g=d===a.length-1;if(g&&b.parent)break;f=Rb(f,a[d]);c=k(c+"/"+a[d]);f.Ya&&(!g||g&&b.pb)&&(f=f.Ya.root);if(!g||b.Wa)for(g=0;40960===(f.mode&61440);)if(f=Wb(c),c=Hb(Db(c),f),f=W(c,{kb:b.kb}).node,40<g++)throw new O(32);}return{path:c,node:f}}
function Xb(a){for(var b;;){if(a===a.parent)return a=a.Ua.rb,b?"/"!==a[a.length-1]?a+"/"+b:a+b:a;b=b?a.name+"/"+b:a.name;a=a.parent}}function Yb(a,b){for(var c=0,d=0;d<b.length;d++)c=(c<<5)-c+b.charCodeAt(d)|0;return(a+c>>>0)%T.length}function Zb(a){var b=Yb(a.parent.id,a.name);if(T[b]===a)T[b]=a.Za;else for(b=T[b];b;){if(b.Za===a){b.Za=a.Za;break}b=b.Za}}
function Rb(a,b){var c;if(c=(c=$b(a,"x"))?c:a.La.lookup?0:2)throw new O(c,a);for(c=T[Yb(a.id,b)];c;c=c.Za){var d=c.name;if(c.parent.id===a.id&&d===b)return c}return a.La.lookup(a,b)}function Pb(a,b,c,d){a=new ac(a,b,c,d);b=Yb(a.parent.id,a.name);a.Za=T[b];return T[b]=a}function Q(a){return 16384===(a&61440)}var bc={r:0,rs:1052672,"r+":2,w:577,wx:705,xw:705,"w+":578,"wx+":706,"xw+":706,a:1089,ax:1217,xa:1217,"a+":1090,"ax+":1218,"xa+":1218};
function cc(a){var b=["r","w","rw"][a&3];a&512&&(b+="w");return b}function $b(a,b){if(Vb)return 0;if(-1===b.indexOf("r")||a.mode&292){if(-1!==b.indexOf("w")&&!(a.mode&146)||-1!==b.indexOf("x")&&!(a.mode&73))return 2}else return 2;return 0}function dc(a,b){try{return Rb(a,b),20}catch(c){}return $b(a,"wx")}function ec(a,b,c){try{var d=Rb(a,b)}catch(f){return f.Na}if(a=$b(a,"wx"))return a;if(c){if(!Q(d.mode))return 54;if(d===d.parent||"/"===Xb(d))return 10}else if(Q(d.mode))return 31;return 0}
function fc(a){var b=4096;for(a=a||0;a<=b;a++)if(!S[a])return a;throw new O(33);}function hc(a,b){ic||(ic=function(){},ic.prototype={});var c=new ic,d;for(d in a)c[d]=a[d];a=c;b=fc(b);a.fd=b;return S[b]=a}var Ob={open:function(a){a.Ma=Tb[a.node.rdev].Ma;a.Ma.open&&a.Ma.open(a)},Xa:function(){throw new O(70);}};function Kb(a,b){Tb[a]={Ma:b}}
function jc(a,b){var c="/"===b,d=!b;if(c&&Sb)throw new O(10);if(!c&&!d){var f=W(b,{pb:!1});b=f.path;f=f.node;if(f.Ya)throw new O(10);if(!Q(f.mode))throw new O(54);}b={type:a,Nb:{},rb:b,Fb:[]};a=a.Ua(b);a.Ua=b;b.root=a;c?Sb=a:f&&(f.Ya=b,f.Ua&&f.Ua.Fb.push(b))}function ba(a,b,c){var d=W(a,{parent:!0}).node;a=Eb(a);if(!a||"."===a||".."===a)throw new O(28);var f=dc(d,a);if(f)throw new O(f);if(!d.La.ab)throw new O(63);return d.La.ab(d,a,b,c)}function X(a,b){ba(a,(void 0!==b?b:511)&1023|16384,0)}
function kc(a,b,c){"undefined"===typeof c&&(c=b,b=438);ba(a,b|8192,c)}function lc(a,b){if(!Hb(a))throw new O(44);var c=W(b,{parent:!0}).node;if(!c)throw new O(44);b=Eb(b);var d=dc(c,b);if(d)throw new O(d);if(!c.La.symlink)throw new O(63);c.La.symlink(c,b,a)}
function sa(a){var b=W(a,{parent:!0}).node,c=Eb(a),d=Rb(b,c),f=ec(b,c,!1);if(f)throw new O(f);if(!b.La.unlink)throw new O(63);if(d.Ya)throw new O(10);try{V.willDeletePath&&V.willDeletePath(a)}catch(g){F("FS.trackingDelegate['willDeletePath']('"+a+"') threw an exception: "+g.message)}b.La.unlink(b,c);Zb(d);try{if(V.onDeletePath)V.onDeletePath(a)}catch(g){F("FS.trackingDelegate['onDeletePath']('"+a+"') threw an exception: "+g.message)}}
function Wb(a){a=W(a).node;if(!a)throw new O(44);if(!a.La.readlink)throw new O(28);return Hb(Xb(a.parent),a.La.readlink(a))}function mc(a,b){a=W(a,{Wa:!b}).node;if(!a)throw new O(44);if(!a.La.Sa)throw new O(63);return a.La.Sa(a)}function nc(a){return mc(a,!0)}function ca(a,b){var c;"string"===typeof a?c=W(a,{Wa:!0}).node:c=a;if(!c.La.Ra)throw new O(63);c.La.Ra(c,{mode:b&4095|c.mode&-4096,timestamp:Date.now()})}
function oc(a){var b;"string"===typeof a?b=W(a,{Wa:!0}).node:b=a;if(!b.La.Ra)throw new O(63);b.La.Ra(b,{timestamp:Date.now()})}function pc(a,b){if(0>b)throw new O(28);var c;"string"===typeof a?c=W(a,{Wa:!0}).node:c=a;if(!c.La.Ra)throw new O(63);if(Q(c.mode))throw new O(31);if(32768!==(c.mode&61440))throw new O(28);if(a=$b(c,"w"))throw new O(a);c.La.Ra(c,{size:b,timestamp:Date.now()})}
function m(a,b,c,d){if(""===a)throw new O(44);if("string"===typeof b){var f=bc[b];if("undefined"===typeof f)throw Error("Unknown file open mode: "+b);b=f}c=b&64?("undefined"===typeof c?438:c)&4095|32768:0;if("object"===typeof a)var g=a;else{a=k(a);try{g=W(a,{Wa:!(b&131072)}).node}catch(n){}}f=!1;if(b&64)if(g){if(b&128)throw new O(20);}else g=ba(a,c,0),f=!0;if(!g)throw new O(44);8192===(g.mode&61440)&&(b&=-513);if(b&65536&&!Q(g.mode))throw new O(54);if(!f&&(c=g?40960===(g.mode&61440)?32:Q(g.mode)&&
("r"!==cc(b)||b&512)?31:$b(g,cc(b)):44))throw new O(c);b&512&&pc(g,0);b&=-131713;d=hc({node:g,path:Xb(g),flags:b,seekable:!0,position:0,Ma:g.Ma,Kb:[],error:!1},d);d.Ma.open&&d.Ma.open(d);!e.logReadFiles||b&1||(Qc||(Qc={}),a in Qc||(Qc[a]=1,F("FS.trackingDelegate error on read file: "+a)));try{V.onOpenFile&&(g=0,1!==(b&2097155)&&(g|=1),0!==(b&2097155)&&(g|=2),V.onOpenFile(a,g))}catch(n){F("FS.trackingDelegate['onOpenFile']('"+a+"', flags) threw an exception: "+n.message)}return d}
function ea(a){if(null===a.fd)throw new O(8);a.ib&&(a.ib=null);try{a.Ma.close&&a.Ma.close(a)}catch(b){throw b;}finally{S[a.fd]=null}a.fd=null}function Rc(a,b,c){if(null===a.fd)throw new O(8);if(!a.seekable||!a.Ma.Xa)throw new O(70);if(0!=c&&1!=c&&2!=c)throw new O(28);a.position=a.Ma.Xa(a,b,c);a.Kb=[]}
function Tc(a,b,c,d,f){if(0>d||0>f)throw new O(28);if(null===a.fd)throw new O(8);if(1===(a.flags&2097155))throw new O(8);if(Q(a.node.mode))throw new O(31);if(!a.Ma.read)throw new O(28);var g="undefined"!==typeof f;if(!g)f=a.position;else if(!a.seekable)throw new O(70);b=a.Ma.read(a,b,c,d,f);g||(a.position+=b);return b}
function da(a,b,c,d,f,g){if(0>d||0>f)throw new O(28);if(null===a.fd)throw new O(8);if(0===(a.flags&2097155))throw new O(8);if(Q(a.node.mode))throw new O(31);if(!a.Ma.write)throw new O(28);a.seekable&&a.flags&1024&&Rc(a,0,2);var n="undefined"!==typeof f;if(!n)f=a.position;else if(!a.seekable)throw new O(70);b=a.Ma.write(a,b,c,d,f,g);n||(a.position+=b);try{if(a.path&&V.onWriteToFile)V.onWriteToFile(a.path)}catch(t){F("FS.trackingDelegate['onWriteToFile']('"+a.path+"') threw an exception: "+t.message)}return b}
function ra(a){var b={encoding:"binary"};b=b||{};b.flags=b.flags||"r";b.encoding=b.encoding||"binary";if("utf8"!==b.encoding&&"binary"!==b.encoding)throw Error('Invalid encoding type "'+b.encoding+'"');var c,d=m(a,b.flags);a=mc(a).size;var f=new Uint8Array(a);Tc(d,f,0,a,0);"utf8"===b.encoding?c=Za(f,0):"binary"===b.encoding&&(c=f);ea(d);return c}
function Uc(){O||(O=function(a,b){this.node=b;this.Jb=function(c){this.Na=c};this.Jb(a);this.message="FS error"},O.prototype=Error(),O.prototype.constructor=O,[44].forEach(function(a){Qb[a]=new O(a);Qb[a].stack="<generic error, no stack>"}))}var Vc;function aa(a,b){var c=0;a&&(c|=365);b&&(c|=146);return c}
function Wc(a,b,c){a=k("/dev/"+a);var d=aa(!!b,!!c);Xc||(Xc=64);var f=Xc++<<8|0;Kb(f,{open:function(g){g.seekable=!1},close:function(){c&&c.buffer&&c.buffer.length&&c(10)},read:function(g,n,t,w){for(var u=0,C=0;C<w;C++){try{var H=b()}catch(ha){throw new O(29);}if(void 0===H&&0===u)throw new O(6);if(null===H||void 0===H)break;u++;n[t+C]=H}u&&(g.node.timestamp=Date.now());return u},write:function(g,n,t,w){for(var u=0;u<w;u++)try{c(n[t+u])}catch(C){throw new O(29);}w&&(g.node.timestamp=Date.now());return u}});
kc(a,d,f)}var Xc,Y={},ic,Qc,Yc={};
function Zc(a,b,c){try{var d=a(b)}catch(f){if(f&&f.node&&k(b)!==k(Xb(f.node)))return-54;throw f;}I[c>>2]=d.dev;I[c+4>>2]=0;I[c+8>>2]=d.ino;I[c+12>>2]=d.mode;I[c+16>>2]=d.nlink;I[c+20>>2]=d.uid;I[c+24>>2]=d.gid;I[c+28>>2]=d.rdev;I[c+32>>2]=0;J=[d.size>>>0,(K=d.size,1<=+La(K)?0<K?(Ma(+Na(K/4294967296),4294967295)|0)>>>0:~~+Oa((K-+(~~K>>>0))/4294967296)>>>0:0)];I[c+40>>2]=J[0];I[c+44>>2]=J[1];I[c+48>>2]=4096;I[c+52>>2]=d.blocks;I[c+56>>2]=d.atime.getTime()/1E3|0;I[c+60>>2]=0;I[c+64>>2]=d.mtime.getTime()/
1E3|0;I[c+68>>2]=0;I[c+72>>2]=d.ctime.getTime()/1E3|0;I[c+76>>2]=0;J=[d.ino>>>0,(K=d.ino,1<=+La(K)?0<K?(Ma(+Na(K/4294967296),4294967295)|0)>>>0:~~+Oa((K-+(~~K>>>0))/4294967296)>>>0:0)];I[c+80>>2]=J[0];I[c+84>>2]=J[1];return 0}var $c=void 0;function ad(){$c+=4;return I[$c-4>>2]}function Z(a){a=S[a];if(!a)throw new O(8);return a}var bd={};
function cd(){if(!dd){var a={USER:"web_user",LOGNAME:"web_user",PATH:"/",PWD:"/",HOME:"/home/web_user",LANG:("object"===typeof navigator&&navigator.languages&&navigator.languages[0]||"C").replace("-","_")+".UTF-8",_:wa||"./this.program"},b;for(b in bd)a[b]=bd[b];var c=[];for(b in a)c.push(b+"="+a[b]);dd=c}return dd}var dd,ed;za?ed=function(){var a=process.hrtime();return 1E3*a[0]+a[1]/1E6}:"undefined"!==typeof dateNow?ed=dateNow:ed=function(){return performance.now()};
function fd(a){for(var b=ed();ed()-b<a/1E3;);}e._usleep=fd;function ac(a,b,c,d){a||(a=this);this.parent=a;this.Ua=a.Ua;this.Ya=null;this.id=Ub++;this.name=b;this.mode=c;this.La={};this.Ma={};this.rdev=d}Object.defineProperties(ac.prototype,{read:{get:function(){return 365===(this.mode&365)},set:function(a){a?this.mode|=365:this.mode&=-366}},write:{get:function(){return 146===(this.mode&146)},set:function(a){a?this.mode|=146:this.mode&=-147}}});Uc();T=Array(4096);jc(P,"/");X("/tmp");X("/home");X("/home/web_user");
(function(){X("/dev");Kb(259,{read:function(){return 0},write:function(d,f,g,n){return n}});kc("/dev/null",259);Jb(1280,Mb);Jb(1536,Nb);kc("/dev/tty",1280);kc("/dev/tty1",1536);if("object"===typeof crypto&&"function"===typeof crypto.getRandomValues){var a=new Uint8Array(1);var b=function(){crypto.getRandomValues(a);return a[0]}}else if(za)try{var c=require("crypto");b=function(){return c.randomBytes(1)[0]}}catch(d){}b||(b=function(){D("random_device")});Wc("random",b);Wc("urandom",b);X("/dev/shm");
X("/dev/shm/tmp")})();X("/proc");X("/proc/self");X("/proc/self/fd");jc({Ua:function(){var a=Pb("/proc/self","fd",16895,73);a.La={lookup:function(b,c){var d=S[+c];if(!d)throw new O(8);b={parent:null,Ua:{rb:"fake"},La:{readlink:function(){return d.path}}};return b.parent=b}};return a}},"/proc/self/fd");function fa(a,b){var c=Array(la(a)+1);a=ma(a,c,0,c.length);b&&(c.length=a);return c}
var id={c:function(a,b,c,d){D("Assertion failed: "+N(a)+", at: "+[b?N(b):"unknown filename",c,d?N(d):"unknown function"])},b:G,t:function(a,b){wb();a=new Date(1E3*I[a>>2]);I[b>>2]=a.getSeconds();I[b+4>>2]=a.getMinutes();I[b+8>>2]=a.getHours();I[b+12>>2]=a.getDate();I[b+16>>2]=a.getMonth();I[b+20>>2]=a.getFullYear()-1900;I[b+24>>2]=a.getDay();var c=new Date(a.getFullYear(),0,1);I[b+28>>2]=(a.getTime()-c.getTime())/864E5|0;I[b+36>>2]=-(60*a.getTimezoneOffset());var d=(new Date(a.getFullYear(),6,1)).getTimezoneOffset();
c=c.getTimezoneOffset();a=(d!=c&&a.getTimezoneOffset()==Math.min(c,d))|0;I[b+32>>2]=a;a=I[Bb()+(a?4:0)>>2];I[b+40>>2]=a;return b},m:function(a,b){try{a=N(a);if(b&-8)var c=-28;else{var d;(d=W(a,{Wa:!0}).node)?(a="",b&4&&(a+="r"),b&2&&(a+="w"),b&1&&(a+="x"),c=a&&$b(d,a)?-2:0):c=-44}return c}catch(f){return"undefined"!==typeof Y&&f instanceof O||D(f),-f.Na}},z:function(a,b){try{return a=N(a),ca(a,b),0}catch(c){return"undefined"!==typeof Y&&c instanceof O||D(c),-c.Na}},H:function(a){try{return a=N(a),
oc(a),0}catch(b){return"undefined"!==typeof Y&&b instanceof O||D(b),-b.Na}},A:function(a,b){try{var c=S[a];if(!c)throw new O(8);ca(c.node,b);return 0}catch(d){return"undefined"!==typeof Y&&d instanceof O||D(d),-d.Na}},I:function(a){try{var b=S[a];if(!b)throw new O(8);oc(b.node);return 0}catch(c){return"undefined"!==typeof Y&&c instanceof O||D(c),-c.Na}},d:function(a,b,c){$c=c;try{var d=Z(a);switch(b){case 0:var f=ad();return 0>f?-28:m(d.path,d.flags,0,f).fd;case 1:case 2:return 0;case 3:return d.flags;
case 4:return f=ad(),d.flags|=f,0;case 12:return f=ad(),Ka[f+0>>1]=2,0;case 13:case 14:return 0;case 16:case 8:return-28;case 9:return Fb(28),-1;default:return-28}}catch(g){return"undefined"!==typeof Y&&g instanceof O||D(g),-g.Na}},C:function(a,b){try{var c=Z(a);return Zc(mc,c.path,b)}catch(d){return"undefined"!==typeof Y&&d instanceof O||D(d),-d.Na}},n:function(a,b,c){try{var d=S[a];if(!d)throw new O(8);if(0===(d.flags&2097155))throw new O(28);pc(d.node,c);return 0}catch(f){return"undefined"!==typeof Y&&
f instanceof O||D(f),-f.Na}},l:function(a,b){try{if(0===b)return-28;if(b<la("/")+1)return-68;ma("/",M,a,b);return a}catch(c){return"undefined"!==typeof Y&&c instanceof O||D(c),-c.Na}},G:function(){return 0},f:function(){return 42},y:function(a,b){try{return a=N(a),Zc(nc,a,b)}catch(c){return"undefined"!==typeof Y&&c instanceof O||D(c),-c.Na}},B:function(a,b){try{return a=N(a),a=k(a),"/"===a[a.length-1]&&(a=a.substr(0,a.length-1)),X(a,b),0}catch(c){return"undefined"!==typeof Y&&c instanceof O||D(c),
-c.Na}},x:function(a,b,c,d,f,g){try{a:{g<<=12;var n=!1;if(0!==(d&16)&&0!==a%16384)var t=-28;else{if(0!==(d&32)){var w=gd(16384,b);if(!w){t=-48;break a}hd(w,0,b);n=!0}else{var u=S[f];if(!u){t=-8;break a}var C=g;if(0!==(c&2)&&0===(d&2)&&2!==(u.flags&2097155))throw new O(2);if(1===(u.flags&2097155))throw new O(2);if(!u.Ma.bb)throw new O(43);var H=u.Ma.bb(u,a,b,C,c,d);w=H.Hb;n=H.fb}Yc[w]={Eb:w,Db:b,fb:n,fd:f,Gb:c,flags:d,offset:g};t=w}}return t}catch(ha){return"undefined"!==typeof Y&&ha instanceof O||
D(ha),-ha.Na}},w:function(a,b){try{if(-1===(a|0)||0===b)var c=-28;else{var d=Yc[a];if(d&&b===d.Db){var f=S[d.fd];if(d.Gb&2){var g=d.flags,n=d.offset,t=M.slice(a,a+b);f&&f.Ma.cb&&f.Ma.cb(f,t,n,b,g)}Yc[a]=null;d.fb&&ja(d.Eb)}c=0}return c}catch(w){return"undefined"!==typeof Y&&w instanceof O||D(w),-w.Na}},v:function(a,b,c){$c=c;try{var d=N(a),f=ad();return m(d,b,f).fd}catch(g){return"undefined"!==typeof Y&&g instanceof O||D(g),-g.Na}},j:function(a,b,c){try{var d=Z(a);return Tc(d,x,b,c)}catch(f){return"undefined"!==
typeof Y&&f instanceof O||D(f),-f.Na}},J:function(a,b,c){try{a=N(a);if(0>=c)var d=-28;else{var f=Wb(a),g=Math.min(c,la(f)),n=x[b+g];ma(f,M,b,c+1);x[b+g]=n;d=g}return d}catch(t){return"undefined"!==typeof Y&&t instanceof O||D(t),-t.Na}},E:function(a){try{a=N(a);var b=W(a,{parent:!0}).node,c=Eb(a),d=Rb(b,c),f=ec(b,c,!0);if(f)throw new O(f);if(!b.La.rmdir)throw new O(63);if(d.Ya)throw new O(10);try{V.willDeletePath&&V.willDeletePath(a)}catch(g){F("FS.trackingDelegate['willDeletePath']('"+a+"') threw an exception: "+
g.message)}b.La.rmdir(b,c);Zb(d);try{if(V.onDeletePath)V.onDeletePath(a)}catch(g){F("FS.trackingDelegate['onDeletePath']('"+a+"') threw an exception: "+g.message)}return 0}catch(g){return"undefined"!==typeof Y&&g instanceof O||D(g),-g.Na}},g:function(a,b){try{return a=N(a),Zc(mc,a,b)}catch(c){return"undefined"!==typeof Y&&c instanceof O||D(c),-c.Na}},K:function(a){try{return a=N(a),sa(a),0}catch(b){return"undefined"!==typeof Y&&b instanceof O||D(b),-b.Na}},p:function(a,b,c){M.copyWithin(a,b,b+c)},
e:function(a){a>>>=0;var b=M.length;if(2147483648<a)return!1;for(var c=1;4>=c;c*=2){var d=b*(1+.2/c);d=Math.min(d,a+100663296);d=Math.max(16777216,a,d);0<d%65536&&(d+=65536-d%65536);a:{try{Ra.grow(Math.min(2147483648,d)-ab.byteLength+65535>>>16);bb(Ra.buffer);var f=1;break a}catch(g){}f=void 0}if(f)return!0}return!1},r:function(a,b){var c=0;cd().forEach(function(d,f){var g=b+c;f=I[a+4*f>>2]=g;for(g=0;g<d.length;++g)x[f++>>0]=d.charCodeAt(g);x[f>>0]=0;c+=d.length+1});return 0},s:function(a,b){var c=
cd();I[a>>2]=c.length;var d=0;c.forEach(function(f){d+=f.length+1});I[b>>2]=d;return 0},h:function(a){try{var b=Z(a);ea(b);return 0}catch(c){return"undefined"!==typeof Y&&c instanceof O||D(c),c.Na}},q:function(a,b){try{var c=Z(a);x[b>>0]=c.tty?2:Q(c.mode)?3:40960===(c.mode&61440)?7:4;return 0}catch(d){return"undefined"!==typeof Y&&d instanceof O||D(d),d.Na}},o:function(a,b,c,d,f){try{var g=Z(a);a=4294967296*c+(b>>>0);if(-9007199254740992>=a||9007199254740992<=a)return-61;Rc(g,a,d);J=[g.position>>>
0,(K=g.position,1<=+La(K)?0<K?(Ma(+Na(K/4294967296),4294967295)|0)>>>0:~~+Oa((K-+(~~K>>>0))/4294967296)>>>0:0)];I[f>>2]=J[0];I[f+4>>2]=J[1];g.ib&&0===a&&0===d&&(g.ib=null);return 0}catch(n){return"undefined"!==typeof Y&&n instanceof O||D(n),n.Na}},k:function(a){try{var b=Z(a);return b.Ma&&b.Ma.fsync?-b.Ma.fsync(b):0}catch(c){return"undefined"!==typeof Y&&c instanceof O||D(c),c.Na}},F:function(a,b,c,d){try{a:{for(var f=Z(a),g=a=0;g<c;g++){var n=da(f,x,I[b+8*g>>2],I[b+(8*g+4)>>2],void 0);if(0>n){var t=
-1;break a}a+=n}t=a}I[d>>2]=t;return 0}catch(w){return"undefined"!==typeof Y&&w instanceof O||D(w),w.Na}},i:function(a){var b=Date.now();I[a>>2]=b/1E3|0;I[a+4>>2]=b%1E3*1E3|0;return 0},a:Ra,L:function(a,b){if(0===a)return Fb(28),-1;var c=I[a>>2];a=I[a+4>>2];if(0>a||999999999<a||0>c)return Fb(28),-1;0!==b&&(I[b>>2]=0,I[b+4>>2]=0);return fd(1E6*c+a/1E3)},D:function(a){switch(a){case 30:return 16384;case 85:return 131072;case 132:case 133:case 12:case 137:case 138:case 15:case 235:case 16:case 17:case 18:case 19:case 20:case 149:case 13:case 10:case 236:case 153:case 9:case 21:case 22:case 159:case 154:case 14:case 77:case 78:case 139:case 80:case 81:case 82:case 68:case 67:case 164:case 11:case 29:case 47:case 48:case 95:case 52:case 51:case 46:case 79:return 200809;
case 27:case 246:case 127:case 128:case 23:case 24:case 160:case 161:case 181:case 182:case 242:case 183:case 184:case 243:case 244:case 245:case 165:case 178:case 179:case 49:case 50:case 168:case 169:case 175:case 170:case 171:case 172:case 97:case 76:case 32:case 173:case 35:return-1;case 176:case 177:case 7:case 155:case 8:case 157:case 125:case 126:case 92:case 93:case 129:case 130:case 131:case 94:case 91:return 1;case 74:case 60:case 69:case 70:case 4:return 1024;case 31:case 42:case 72:return 32;
case 87:case 26:case 33:return 2147483647;case 34:case 1:return 47839;case 38:case 36:return 99;case 43:case 37:return 2048;case 0:return 2097152;case 3:return 65536;case 28:return 32768;case 44:return 32767;case 75:return 16384;case 39:return 1E3;case 89:return 700;case 71:return 256;case 40:return 255;case 2:return 100;case 180:return 64;case 25:return 20;case 5:return 16;case 6:return 6;case 73:return 4;case 84:return"object"===typeof navigator?navigator.hardwareConcurrency||1:1}Fb(28);return-1},
M:function(a){var b=Date.now()/1E3|0;a&&(I[a>>2]=b);return b},u:function(a,b){if(b){var c=1E3*I[b+8>>2];c+=I[b+12>>2]/1E3}else c=Date.now();a=N(a);try{b=c;var d=W(a,{Wa:!0}).node;d.La.Ra(d,{timestamp:Math.max(b,c)});return 0}catch(f){a=f;if(!(a instanceof O)){a+=" : ";a:{d=Error();if(!d.stack){try{throw Error();}catch(g){d=g}if(!d.stack){d="(no stack trace available)";break a}}d=d.stack.toString()}e.extraStackTrace&&(d+="\n"+e.extraStackTrace());d=vb(d);throw a+d;}Fb(a.Na);return-1}}};
(function(){function a(f){e.asm=f.exports;ib--;e.monitorRunDependencies&&e.monitorRunDependencies(ib);0==ib&&(null!==jb&&(clearInterval(jb),jb=null),kb&&(f=kb,kb=null,f()))}function b(f){a(f.instance)}function c(f){return qb().then(function(g){return WebAssembly.instantiate(g,d)}).then(f,function(g){F("failed to asynchronously prepare wasm: "+g);D(g)})}var d={a:id};ib++;e.monitorRunDependencies&&e.monitorRunDependencies(ib);if(e.instantiateWasm)try{return e.instantiateWasm(d,a)}catch(f){return F("Module.instantiateWasm callback failed with error: "+
f),!1}(function(){if(Ja||"function"!==typeof WebAssembly.instantiateStreaming||nb()||lb("file://")||"function"!==typeof fetch)return c(b);fetch(mb,{credentials:"same-origin"}).then(function(f){return WebAssembly.instantiateStreaming(f,d).then(b,function(g){F("wasm streaming compile failed: "+g);F("falling back to ArrayBuffer instantiation");return c(b)})})})();return{}})();
var rb=e.___wasm_call_ctors=function(){return(rb=e.___wasm_call_ctors=e.asm.N).apply(null,arguments)},hd=e._memset=function(){return(hd=e._memset=e.asm.O).apply(null,arguments)};e._sqlite3_free=function(){return(e._sqlite3_free=e.asm.P).apply(null,arguments)};var Gb=e.___errno_location=function(){return(Gb=e.___errno_location=e.asm.Q).apply(null,arguments)};e._sqlite3_finalize=function(){return(e._sqlite3_finalize=e.asm.R).apply(null,arguments)};
e._sqlite3_reset=function(){return(e._sqlite3_reset=e.asm.S).apply(null,arguments)};e._sqlite3_clear_bindings=function(){return(e._sqlite3_clear_bindings=e.asm.T).apply(null,arguments)};e._sqlite3_value_blob=function(){return(e._sqlite3_value_blob=e.asm.U).apply(null,arguments)};e._sqlite3_value_text=function(){return(e._sqlite3_value_text=e.asm.V).apply(null,arguments)};e._sqlite3_value_bytes=function(){return(e._sqlite3_value_bytes=e.asm.W).apply(null,arguments)};
e._sqlite3_value_double=function(){return(e._sqlite3_value_double=e.asm.X).apply(null,arguments)};e._sqlite3_value_int=function(){return(e._sqlite3_value_int=e.asm.Y).apply(null,arguments)};e._sqlite3_value_type=function(){return(e._sqlite3_value_type=e.asm.Z).apply(null,arguments)};e._sqlite3_result_blob=function(){return(e._sqlite3_result_blob=e.asm._).apply(null,arguments)};e._sqlite3_result_double=function(){return(e._sqlite3_result_double=e.asm.$).apply(null,arguments)};
e._sqlite3_result_error=function(){return(e._sqlite3_result_error=e.asm.aa).apply(null,arguments)};e._sqlite3_result_int=function(){return(e._sqlite3_result_int=e.asm.ba).apply(null,arguments)};e._sqlite3_result_int64=function(){return(e._sqlite3_result_int64=e.asm.ca).apply(null,arguments)};e._sqlite3_result_null=function(){return(e._sqlite3_result_null=e.asm.da).apply(null,arguments)};e._sqlite3_result_text=function(){return(e._sqlite3_result_text=e.asm.ea).apply(null,arguments)};
e._sqlite3_step=function(){return(e._sqlite3_step=e.asm.fa).apply(null,arguments)};e._sqlite3_column_count=function(){return(e._sqlite3_column_count=e.asm.ga).apply(null,arguments)};e._sqlite3_data_count=function(){return(e._sqlite3_data_count=e.asm.ha).apply(null,arguments)};e._sqlite3_column_blob=function(){return(e._sqlite3_column_blob=e.asm.ia).apply(null,arguments)};e._sqlite3_column_bytes=function(){return(e._sqlite3_column_bytes=e.asm.ja).apply(null,arguments)};
e._sqlite3_column_double=function(){return(e._sqlite3_column_double=e.asm.ka).apply(null,arguments)};e._sqlite3_column_text=function(){return(e._sqlite3_column_text=e.asm.la).apply(null,arguments)};e._sqlite3_column_type=function(){return(e._sqlite3_column_type=e.asm.ma).apply(null,arguments)};e._sqlite3_column_name=function(){return(e._sqlite3_column_name=e.asm.na).apply(null,arguments)};e._sqlite3_bind_blob=function(){return(e._sqlite3_bind_blob=e.asm.oa).apply(null,arguments)};
e._sqlite3_bind_double=function(){return(e._sqlite3_bind_double=e.asm.pa).apply(null,arguments)};e._sqlite3_bind_int=function(){return(e._sqlite3_bind_int=e.asm.qa).apply(null,arguments)};e._sqlite3_bind_text=function(){return(e._sqlite3_bind_text=e.asm.ra).apply(null,arguments)};e._sqlite3_bind_parameter_index=function(){return(e._sqlite3_bind_parameter_index=e.asm.sa).apply(null,arguments)};e._sqlite3_errmsg=function(){return(e._sqlite3_errmsg=e.asm.ta).apply(null,arguments)};
e._sqlite3_exec=function(){return(e._sqlite3_exec=e.asm.ua).apply(null,arguments)};e._sqlite3_prepare_v2=function(){return(e._sqlite3_prepare_v2=e.asm.va).apply(null,arguments)};e._sqlite3_changes=function(){return(e._sqlite3_changes=e.asm.wa).apply(null,arguments)};e._sqlite3_close_v2=function(){return(e._sqlite3_close_v2=e.asm.xa).apply(null,arguments)};e._sqlite3_create_function_v2=function(){return(e._sqlite3_create_function_v2=e.asm.ya).apply(null,arguments)};
e._sqlite3_open=function(){return(e._sqlite3_open=e.asm.za).apply(null,arguments)};var Xa=e._malloc=function(){return(Xa=e._malloc=e.asm.Aa).apply(null,arguments)},ja=e._free=function(){return(ja=e._free=e.asm.Ba).apply(null,arguments)};e._RegisterExtensionFunctions=function(){return(e._RegisterExtensionFunctions=e.asm.Ca).apply(null,arguments)};
var Bb=e.__get_tzname=function(){return(Bb=e.__get_tzname=e.asm.Da).apply(null,arguments)},Ab=e.__get_daylight=function(){return(Ab=e.__get_daylight=e.asm.Ea).apply(null,arguments)},yb=e.__get_timezone=function(){return(yb=e.__get_timezone=e.asm.Fa).apply(null,arguments)},ka=e.stackSave=function(){return(ka=e.stackSave=e.asm.Ga).apply(null,arguments)},oa=e.stackRestore=function(){return(oa=e.stackRestore=e.asm.Ha).apply(null,arguments)},r=e.stackAlloc=function(){return(r=e.stackAlloc=e.asm.Ia).apply(null,
arguments)},gd=e._memalign=function(){return(gd=e._memalign=e.asm.Ja).apply(null,arguments)};e.cwrap=function(a,b,c,d){c=c||[];var f=c.every(function(g){return"number"===g});return"string"!==b&&f&&!d?Ta(a):function(){return Ua(a,b,c,arguments)}};e.stackSave=ka;e.stackRestore=oa;e.stackAlloc=r;var jd;kb=function kd(){jd||ld();jd||(kb=kd)};
function ld(){function a(){if(!jd&&(jd=!0,e.calledRun=!0,!Sa)){e.noFSInit||Vc||(Vc=!0,Uc(),e.stdin=e.stdin,e.stdout=e.stdout,e.stderr=e.stderr,e.stdin?Wc("stdin",e.stdin):lc("/dev/tty","/dev/stdin"),e.stdout?Wc("stdout",null,e.stdout):lc("/dev/tty","/dev/stdout"),e.stderr?Wc("stderr",null,e.stderr):lc("/dev/tty1","/dev/stderr"),m("/dev/stdin","r"),m("/dev/stdout","w"),m("/dev/stderr","w"));sb(eb);Vb=!1;sb(fb);if(e.onRuntimeInitialized)e.onRuntimeInitialized();if(e.postRun)for("function"==typeof e.postRun&&
(e.postRun=[e.postRun]);e.postRun.length;){var b=e.postRun.shift();gb.unshift(b)}sb(gb)}}if(!(0<ib)){if(e.preRun)for("function"==typeof e.preRun&&(e.preRun=[e.preRun]);e.preRun.length;)hb();sb(db);0<ib||(e.setStatus?(e.setStatus("Running..."),setTimeout(function(){setTimeout(function(){e.setStatus("")},1);a()},1)):a())}}e.run=ld;if(e.preInit)for("function"==typeof e.preInit&&(e.preInit=[e.preInit]);0<e.preInit.length;)e.preInit.pop()();noExitRuntime=!0;ld();
// The shell-pre.js and emcc-generated code goes above
return Module;
}); // The end of the promise being returned

return initSqlJsPromise;
} // The end of our initSqlJs function

// This bit below is copied almost exactly from what you get when you use the MODULARIZE=1 flag with emcc
// However, we don't want to use the emcc modularization. See shell-pre.js
if (typeof exports === 'object' && typeof module === 'object') {
    module.exports = initSqlJs;
    // This will allow the module to be used in ES6 or CommonJS
    module.exports.default = initSqlJs;
} else if (typeof define === 'function' && define['amd']) {
    define([], function () {
        return initSqlJs;
    });
} else if (typeof exports === 'object') {
    exports["Module"] = initSqlJs;
}

shouldRunNow = true;
that.run = initSqlJs;





}
return WasmRunner;
});/* global initSqlJs */
/* eslint-env worker */
/* eslint no-restricted-globals: ["error"] */

"use strict";

var db;

function onModuleReady(SQL) {
    function createDb(data) {
        if (db != null) db.close();
        db = new SQL.Database(data);
        return db;
    }

    var buff; var data; var result;
    data = this["data"];
    switch (data && data["action"]) {
        case "open":
            buff = data["buffer"];
            createDb(buff && new Uint8Array(buff));
            return postMessage({
                id: data["id"],
                ready: true
            });
        case "exec":
            if (db === null) {
                createDb();
            }
            if (!data["sql"]) {
                throw "exec: Missing query string";
            }
            return postMessage({
                id: data["id"],
                results: db.exec(data["sql"], data["params"])
            });
        case "each":
            if (db === null) {
                createDb();
            }
            var callback = function callback(row) {
                return postMessage({
                    id: data["id"],
                    row: row,
                    finished: false
                });
            };
            var done = function done() {
                return postMessage({
                    id: data["id"],
                    finished: true
                });
            };
            return db.each(data["sql"], data["params"], callback, done);
        case "export":
            buff = db["export"]();
            result = {
                id: data["id"],
                buffer: buff
            };
            try {
                return postMessage(result, [result]);
            } catch (error) {
                return postMessage(result);
            }
        case "close":
            if (db) {
                db.close();
            }
            return postMessage({
                id: data["id"]
            });
        default:
            throw new Error("Invalid action : " + (data && data["action"]));
    }
}

function onError(err) {
    return postMessage({
        id: this["data"]["id"],
        error: err["message"]
    });
}

if (typeof importScripts === "function") {
    db = null;
    var sqlModuleReady = initSqlJs();
    self.onmessage = function onmessage(event) {
        return sqlModuleReady
            .then(onModuleReady.bind(event))
            .catch(onError.bind(event));
    };
}
