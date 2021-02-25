#pragma once
#include <string>
#include <ostream>
#include <vector>
#include <stdint.h>     // For unit_32/64 and co

#define LDE_TOOLS_API
class JSONPrecision;
class JSONContainer;



class LDE_TOOLS_API JSONWriter{
  friend class JSONPrecision;
  typedef JSONWriter JSONWriterImpl;
 private:

 friend class JSONContainer;
 friend class JSONContainerList;
 friend class JSONContainerMap;
 JSONWriter();
 public:
 JSONWriter(std::ostream& theStream);
 ~JSONWriter();

 void key(const std::string & theName);

 void valueNull();
 void value(const std::string& theString);
 void value(const char* s);
 void value(bool theValue);
 void value(double val);
 void value(int64_t val);
 void value(int val);
 void value(unsigned val);
 void value(uint64_t val);

 template<typename T> void pair(const std::string & name, const T & val){
   key(name);
   value(val);
 }

 inline JSONWriterImpl & Impl() { return *this; }

 private:
 void startList();
 void endList();
 void startMap();
 void endMap();
 void separator(){
   itsStream<<",";
 };
 std::ostream& itsStream;
 bool itsWroteKey;
 std::vector<bool> itsOpenTags;
};
class LDE_TOOLS_API JSONContainerList
  {
  private:
  JSONContainerList();
  public:
  JSONContainerList(JSONWriter& theWriter):itsWriter(theWriter){
    itsWriter.startList();
  };
  ~JSONContainerList(){
    itsWriter.endList();
  };
  private:
  JSONWriter& itsWriter;
  };
#define JSON_LIST(writer) JSONContainerList openLIST##__LINE__(writer)
class LDE_TOOLS_API JSONContainerMap
  {
  private:
  JSONContainerMap();
  public:
  JSONContainerMap(JSONWriter& theWriter):itsWriter(theWriter){
    itsWriter.startMap();
  };
  ~JSONContainerMap(){
    itsWriter.endMap();
  };
  private:
  JSONWriter& itsWriter;
  };
#define JSON_MAP(writer) JSONContainerMap openMAP##__LINE__(writer)
class LDE_TOOLS_API JSONPrecision
  {
    JSONPrecision();
  public:
  JSONPrecision(JSONWriter& theWriter, std::streamsize prec):itsWriter(theWriter){};
  ~JSONPrecision(){};
  private:
  JSONWriter&      itsWriter;
  int m_previousPrecision;
  };
#define WRITER_PRECISION(writer, value) JSONPrecision precision##__LINE__(writer, value)
typedef JSONWriter JSONWriterImpl;
typedef std::shared_ptr<JSONWriterImpl> JSONWriterImpl_SharedPtr;
