#pragma once

//wasm #include <Tools/Tools.h>
#include <memory>
//wasm #include <boost/filesystem.hpp>

#include <iosfwd>
#include <stdint.h> // int64_t

class JsonDocumentWrapper;

#define LDE_TOOLS_API       //wasm
class LDE_TOOLS_API JSONGenericObject
{
public:
	JSONGenericObject(const JSONGenericObject&);
	JSONGenericObject& operator=(const JSONGenericObject& rhs);

	JSONGenericObject(); // NULL object

	JSONGenericObject(std::shared_ptr<JsonDocumentWrapper> document);
	~JSONGenericObject();

	void stringify(std::ostream& ostream) const;

	operator bool() const;

	///////////////////////////////////
	// Objects access
	const JSONGenericObject getObject(const std::string& key) const;
	const JSONGenericObject getArray(const std::string& key) const;

	///////////////////////////////////
	// Values access
	bool        haveValue(const std::string& key) const;

	bool        isDouble(const std::string& key) const;
	bool        isString(const std::string& key) const;
	bool        isBool(const std::string& key) const;
	bool        isInteger(const std::string& key) const;
	bool        isNull(const std::string& key) const;

	double      getDouble(const std::string& key) const;
	std::string getString(const std::string& key) const;
	bool        getBool(const std::string& key) const;
	int64_t     getInteger(const std::string& key) const;

	///////////////////////////////////
	// Array access
	size_t                   size() const;
	const JSONGenericObject  operator[](size_t index) const;

	bool        isDouble(size_t index) const;
	bool        isString(size_t index) const;
	bool        isBool(size_t index) const;
	bool        isInteger(size_t index) const;
	bool        isNull(size_t index) const;

	double      getDouble(size_t index) const;
	std::string getString(size_t index) const;
	bool        getBool(size_t index) const;
	int64_t     getInteger(size_t index) const;

private:
	template<typename EX, typename T>
	static void JSONThrow(const JSONGenericObject& obj, const char* paramName, const T& paramVal, const std::string& msg, const char*const fct);
	static void checkObjectT(const JSONGenericObject& obj, const std::string& key, bool checkKey, const char*const fct);
	static void checkArrayT(const JSONGenericObject& obj, size_t index, const char*const fct);

private:
	std::shared_ptr<JsonDocumentWrapper>	m_jsonDocument;
};

class LDE_TOOLS_API JSONReader
{
public:
	JSONReader(const JSONReader&);
	JSONReader& operator=(const JSONReader& rhs);

	JSONReader();
	~JSONReader();

	// Return the root node of the DOM
	const JSONGenericObject parse(std::istream& stream);
    //wasm const JSONGenericObject parse(const boost::filesystem::path& file);
	const JSONGenericObject parse(const std::string& string);

	void stringify(std::ostream& ostream) const;

private:
	std::shared_ptr<JsonDocumentWrapper>	m_jsonDocument;
};
