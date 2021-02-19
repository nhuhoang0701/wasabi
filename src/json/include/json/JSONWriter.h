#pragma once

#ifdef __LED_TOOLS_JSON_
#error "pragma once not supported"
#endif
#define __LED_TOOLS_JSON_

#include <Tools/Tools.h>
#include <boost/shared_ptr.hpp>

#include <string>
#include <ostream>
#include <stdint.h>     // For unit_32/64 and co

class bo_utf8string; // From #include <srvtools/kcputf8string.h>

class JSONPrecision;
class JSONContainer;
class JSONWriterImpl;
typedef boost::shared_ptr<JSONWriterImpl> JSONWriterImpl_SharedPtr;

class LDE_TOOLS_API JSONWriter
{
	friend class JSONPrecision;
private:
	friend class JSONContainer;

	JSONWriter();
public:
	JSONWriter(std::ostream& stream);
	~JSONWriter();

	void key(const std::string & name);

	void valueNull();
	void value(const std::string& s);
	void value(const bo_utf8string& s);
	void value(const char* s);
	void value(bool val);
	void value(double val);
	void value(int64_t val);
	void value(int val);
	void value(unsigned val);
	void value(uint64_t val);
#if defined(__clang) && defined(_WIN32)
	void value(long val);
#endif

	template<typename T>
	inline void pair(const std::string & name, const T & val)
	{
		key(name);
		value(val);
	}

	inline JSONWriterImpl & Impl() { return *m_writer; }

private:
	JSONWriterImpl_SharedPtr m_writer;
};

class LDE_TOOLS_API JSONContainerList
{
private:
	JSONContainerList();

public:
	JSONContainerList(JSONWriter& writer);
	~JSONContainerList();

private:
	JSONWriter&    m_writer;
};
#define JSON_LIST(writer) JSONContainerList openLIST##__LINE__(writer)

class LDE_TOOLS_API JSONContainerMap
{
private:
	JSONContainerMap();

public:
	JSONContainerMap(JSONWriter& writer);
	~JSONContainerMap();

private:
	JSONWriter&    m_writer;
};
#define JSON_MAP(writer) JSONContainerMap openMAP##__LINE__(writer)


class LDE_TOOLS_API JSONPrecision
{
	JSONPrecision();
public:
	JSONPrecision(JSONWriter& writer, std::streamsize prec);
	~JSONPrecision();

private:
	JSONWriter&      m_writer;
	int				 m_previousPrecision;
};
#define WRITER_PRECISION(writer, value) JSONPrecision precision##__LINE__(writer, value)
