#include "json/jsonWriter.h"
#include <iomanip>
#include <string_view>
using namespace std;

namespace wasabiUtils{

  void escape_json(string_view theString, ostream& theStream) {
    theStream << "\"";
    for(
        auto c = theString.cbegin(); c != theString.cend(); c++
        ) {
      switch (*c) {
      case '"': theStream << "\\\""; break;
      case '\\': theStream << "\\\\"; break;
      case '\b': theStream << "\\b"; break;
      case '\f': theStream << "\\f"; break;
      case '\n': theStream << "\\n"; break;
      case '\r': theStream << "\\r"; break;
      case '\t': theStream << "\\t"; break;
      default:
        if ('\x00' <= *c && *c <= '\x1f') {
          theStream << "\\u"
                    << hex << setw(4) << setfill('0') << (int)*c;
        } else {
          theStream << *c;
        }
      }
    }
    theStream << "\"";
  }
}
JSONWriter::JSONWriter(ostream& theStream):itsStream(theStream),itsWroteKey(false){}
JSONWriter::~JSONWriter(){};
void JSONWriter::key(const string& theKey){
  if(itsOpenTags.size()>0 && itsOpenTags.at(itsOpenTags.size()-1)){
    itsOpenTags.at(itsOpenTags.size()-1)=false;
    itsStream<< ",";
  }
  wasabiUtils::escape_json(theKey, itsStream);
  itsWroteKey=true;
}
void JSONWriter::valueNull(){
  if(itsWroteKey){
    itsStream << ":";
    itsWroteKey = false;
  }
  if(itsOpenTags.size()>0){
    if(
       itsOpenTags.at(itsOpenTags.size()-1)
       ){
      itsStream<< ",";
    } else{
      itsOpenTags.at(itsOpenTags.size()-1) = true;
    }
  }
  itsStream << "null";
}
void JSONWriter::value(const std::string& theString){
  if(itsWroteKey){
    itsStream << ":";
    itsWroteKey = false;
  }
  if(itsOpenTags.size()>0){
    if(
       itsOpenTags.at(itsOpenTags.size()-1)
       ){
      itsStream<< ",";
    } else{
      itsOpenTags.at(itsOpenTags.size()-1) = true;
    }
  }
  wasabiUtils::escape_json(theString, itsStream);
};
void JSONWriter::value(const char* theCharPointer){
  if(itsWroteKey){
    itsStream << ":";
    itsWroteKey = false;
  }
  if(itsOpenTags.size()>0){
    if(
       itsOpenTags.at(itsOpenTags.size()-1)
       ){
      itsStream<< ",";
    } else{
      itsOpenTags.at(itsOpenTags.size()-1) = true;
    }
  }
  wasabiUtils::escape_json(theCharPointer,itsStream);
};
void JSONWriter::value(bool theValue){
  if(itsWroteKey){
    itsStream << ":";
    itsWroteKey = false;
  }
  if(itsOpenTags.size()>0){
    if(
       itsOpenTags.at(itsOpenTags.size()-1)
       ){
      itsStream<< ",";
    } else{
      itsOpenTags.at(itsOpenTags.size()-1) = true;
    }
  }
  itsStream << (theValue?"true":"false");
};
void JSONWriter::value(int64_t theValue){
  if(itsWroteKey){
    itsStream << ":";
    itsWroteKey = false;
  }
  if(itsOpenTags.size()>0){
    if(
       itsOpenTags.at(itsOpenTags.size()-1)
       ){
      itsStream<< ",";
    } else{
      itsOpenTags.at(itsOpenTags.size()-1) = true;
    }
  }
  itsStream << theValue;
}
void JSONWriter::value(int theValue){
  if(itsWroteKey){
    itsStream << ":";
    itsWroteKey = false;
  }
  if(itsOpenTags.size()>0){
    if(
       itsOpenTags.at(itsOpenTags.size()-1)
       ){
      itsStream<< ",";
    } else{
      itsOpenTags.at(itsOpenTags.size()-1) = true;
    }
  }
  itsStream<<theValue;
}
void JSONWriter::value(unsigned theValue){
  if(itsWroteKey){
    itsStream << ":";
    itsWroteKey = false;
  }
  if(itsOpenTags.size()>0){
    if(
       itsOpenTags.at(itsOpenTags.size()-1)
       ){
      itsStream<< ",";
    } else{
      itsOpenTags.at(itsOpenTags.size()-1) = true;
    }
  }
  itsStream << theValue;
}
void JSONWriter::value(uint64_t theValue){
  if(itsWroteKey){
    itsStream << ":";
    itsWroteKey = false;
  }
  if(itsOpenTags.size()>0){
    if(
       itsOpenTags.at(itsOpenTags.size()-1)
       ){
      itsStream<< ",";
    } else{
      itsOpenTags.at(itsOpenTags.size()-1) = true;
    }
  }
  itsStream<<theValue;
}
