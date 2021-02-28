#include "json/jsonWriter.h"
#include <iomanip>
#include <ostream>
#include <string_view>
#include <vector>
using namespace std;
namespace wasabiUtils{
  struct JsonWriterImpl{
    JsonWriterImpl(
                   ostream& theStream
                   ):itsStream(theStream),
                     itsWroteKey(false)
    {
    };
    bool itsWroteKey;
    vector<bool> itsOpenTags;
    ostream& itsStream;
  };
  void escape_json(string_view theString, ostream& theStream) {
    theStream << "\"";
    for_each(
             theString.cbegin(),theString.cend(),
             [&theStream](char c)  -> void {
               switch (c) {
               case '"': theStream << "\\\""; break;
               case '\\': theStream << "\\\\"; break;
               case '\b': theStream << "\\b"; break;
               case '\f': theStream << "\\f"; break;
               case '\n': theStream << "\\n"; break;
               case '\r': theStream << "\\r"; break;
               case '\t': theStream << "\\t"; break;
               default:
                 if ('\x00' <= c && c <= '\x1f') {
                   theStream << "\\u"
                             << hex << setw(4) << setfill('0') << (int)c;
                 } else {
                   theStream << c;
                 }
               }
             }
             );
    theStream << "\"";
  }
}
JSONWriter::JSONWriter(
                       ostream& theStream
                       ):itsPimpl(
                                  new wasabiUtils::JsonWriterImpl(theStream)
                                  ){
}
JSONWriter::~JSONWriter(){};
void JSONWriter::startList(){
  if(itsPimpl->itsWroteKey){
    itsPimpl->itsStream << ":";
    itsPimpl->itsWroteKey = false;
  }
  if(itsPimpl->itsOpenTags.size()>0){
    if(
       itsPimpl->itsOpenTags.at(itsPimpl->itsOpenTags.size()-1)
       ){
      itsPimpl->itsStream<< ",";
    } else{
      itsPimpl->itsOpenTags.at(itsPimpl->itsOpenTags.size()-1) = true;
    }
  }
  itsPimpl->itsOpenTags.push_back(false);
  itsPimpl->itsStream << "[";
}
void JSONWriter::endList(){
  itsPimpl->itsOpenTags.pop_back();
  itsPimpl->itsStream<<"]";
};
void JSONWriter::endMap(){
  itsPimpl->itsOpenTags.pop_back();
  itsPimpl->itsStream<<"}";
}
void JSONWriter::startMap(){
  wasabiUtils::JsonWriterImpl& aImpl = *itsPimpl;
  if(aImpl.itsWroteKey){
    aImpl.itsStream << ":";
    aImpl.itsWroteKey = false;
  }
  if(aImpl.itsOpenTags.size()>0){
    if(
       aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1)
       ){
      aImpl.itsStream<< ",";
    } else{
      aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1) = true;
    }
  }
  aImpl.itsOpenTags.push_back(false);
  aImpl.itsStream<<"{";
}
void JSONWriter::key(string_view theKey){
  wasabiUtils::JsonWriterImpl& aImpl = *itsPimpl;
  if(aImpl.itsOpenTags.size()>0 && aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1)){
    aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1)=false;
    aImpl.itsStream<< ",";
  }
  wasabiUtils::escape_json(theKey, aImpl.itsStream);
  aImpl.itsWroteKey=true;
}
void JSONWriter::separator(){
  wasabiUtils::JsonWriterImpl& aImpl = *itsPimpl;
  aImpl.itsStream<<",";
};
void JSONWriter::valueNull(){
  wasabiUtils::JsonWriterImpl& aImpl = *itsPimpl;
  if(aImpl.itsWroteKey){
    aImpl.itsStream << ":";
    aImpl.itsWroteKey = false;
  }
  if(aImpl.itsOpenTags.size()>0){
    if(
       aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1)
       ){
      aImpl.itsStream<< ",";
    } else{
      aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1) = true;
    }
  }
  aImpl.itsStream << "null";
}
void JSONWriter::value(string_view theString){
  wasabiUtils::JsonWriterImpl& aImpl = *itsPimpl;
  if(aImpl.itsWroteKey){
    aImpl.itsStream << ":";
    aImpl.itsWroteKey = false;
  }
  if(aImpl.itsOpenTags.size()>0){
    if(
       aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1)
       ){
      aImpl.itsStream<< ",";
    } else{
      aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1) = true;
    }
  }
  wasabiUtils::escape_json(theString, aImpl.itsStream);
};
void JSONWriter::value(const char* theCharPointer){
  wasabiUtils::JsonWriterImpl& aImpl = *itsPimpl;
  if(aImpl.itsWroteKey){
    aImpl.itsStream << ":";
    aImpl.itsWroteKey = false;
  }
  if(aImpl.itsOpenTags.size()>0){
    if(
       aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1)
       ){
      aImpl.itsStream<< ",";
    } else{
      aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1) = true;
    }
  }
  wasabiUtils::escape_json(theCharPointer,aImpl.itsStream);
};
void JSONWriter::value(bool theValue){
  wasabiUtils::JsonWriterImpl& aImpl = *itsPimpl;
  if(aImpl.itsWroteKey){
    aImpl.itsStream << ":";
    aImpl.itsWroteKey = false;
  }
  if(aImpl.itsOpenTags.size()>0){
    if(
       aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1)
       ){
      aImpl.itsStream<< ",";
    } else{
      aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1) = true;
    }
  }
  aImpl.itsStream << (theValue?"true":"false");
};
void JSONWriter::value(int64_t theValue){
  wasabiUtils::JsonWriterImpl& aImpl = *itsPimpl;
  if(aImpl.itsWroteKey){
    aImpl.itsStream << ":";
    aImpl.itsWroteKey = false;
  }
  if(aImpl.itsOpenTags.size()>0){
    if(
       aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1)
       ){
      aImpl.itsStream<< ",";
    } else{
      aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1) = true;
    }
  }
  aImpl.itsStream << theValue;
}
void JSONWriter::value(int theValue){
  wasabiUtils::JsonWriterImpl& aImpl = *itsPimpl;
  if(aImpl.itsWroteKey){
    aImpl.itsStream << ":";
    aImpl.itsWroteKey = false;
  }
  if(aImpl.itsOpenTags.size()>0){
    if(
       aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1)
       ){
      aImpl.itsStream<< ",";
    } else{
      aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1) = true;
    }
  }
  aImpl.itsStream<<theValue;
}
void JSONWriter::value(unsigned theValue){
  wasabiUtils::JsonWriterImpl& aImpl = *itsPimpl;
  if(aImpl.itsWroteKey){
    aImpl.itsStream << ":";
    aImpl.itsWroteKey = false;
  }
  if(aImpl.itsOpenTags.size()>0){
    if(
       aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1)
       ){
      aImpl.itsStream<< ",";
    } else{
      aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1) = true;
    }
  }
  aImpl.itsStream << theValue;
}
void JSONWriter::value(uint64_t theValue){
  wasabiUtils::JsonWriterImpl& aImpl = *itsPimpl;
  if(aImpl.itsWroteKey){
    aImpl.itsStream << ":";
    aImpl.itsWroteKey = false;
  }
  if(aImpl.itsOpenTags.size()>0){
    if(
       aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1)
       ){
      aImpl.itsStream<< ",";
    } else{
      aImpl.itsOpenTags.at(aImpl.itsOpenTags.size()-1) = true;
    }
  }
  aImpl.itsStream<<theValue;
}
