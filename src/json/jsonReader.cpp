#include "json/jsonReader.h"

#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cmath> //std::modf

//extern "C"
//{
#include <cJSON/cJSON.h>
// https://github.com/DaveGamble/cJSON
//}

using JsonObject = cJSON;

// the purpose of this doc handle is to manage the root object
class JsonDocument
{
public:
	explicit JsonDocument() : m_root(NULL) {}
	explicit JsonDocument(JsonObject* doc) : m_root(doc) {}

	JsonDocument(const JsonDocument&) = delete;
	JsonDocument& operator=(const JsonDocument&) = delete;

	~JsonDocument()
	{
		if (m_root)
		{
			cJSON_Delete(m_root);
			m_root = NULL;
		}
	}

	const JsonObject* get() const	{ return m_root;}

private:
	JsonObject* m_root;
};



class JsonDocumentWrapper
{
public:
	inline JsonDocumentWrapper()
		: m_document(NULL), m_object(NULL)
	{
	}

	inline JsonDocumentWrapper(JsonObject * jsonObject)
		: m_object(jsonObject)
	{
		if (!m_object)
			throw std::invalid_argument("m_object is NULL");
	}

	inline ~JsonDocumentWrapper()
	{
		// m_object is only a pointer into the document
		m_object = NULL;
	}

	JsonDocumentWrapper & operator=(const JsonDocumentWrapper & rhs)
	{
		m_object = rhs.m_object;
		m_document = rhs.m_document;
		return *this;
	}

	const JsonObject * getJsonObject() const
	{
		if (!m_object)
			throw std::logic_error("JsonDocumentWrapper::getJsonObject() current object is NULL");
		if (! cJSON_IsObject(m_object))
			return NULL;
		return m_object;
	}

	const JsonObject * getJsonArray() const
	{
		if (!m_object)
			throw std::logic_error("JsonDocumentWrapper::getJsonArray() current array is NULL");
		if (! cJSON_IsArray(m_object))
			return NULL;
		return m_object;
	}

	inline JsonObject * Object() const
	{
		return m_object;
	}

	inline JsonObject & ObjectRef() const
	{
		if (!m_object)
			throw std::invalid_argument("m_object is NULL");
		return *m_object;
	}

	inline JsonDocument & Document() const
	{
		if (!m_document)
			throw std::invalid_argument("m_document is NULL");
		return *m_document;
	}

	void stringify(std::ostream & ostream) const
	{
		if (!m_object)
			return;

		char* str = NULL;

		if (m_document)
			str = cJSON_Print(m_document->get());
		else
			str = cJSON_Print(m_object);

		ostream << str;

		cJSON_free(str);
	}

	void parse(std::istream& stream)
	{
		if (stream.peek() != std::ifstream::traits_type::eof()) // empty stream?
		{
			std::istreambuf_iterator<char> eos;
			const std::string str(std::istreambuf_iterator<char>(stream), eos);

			parse(str.c_str());
		}
	}

	void parse(const std::string & str)
	{
		if (!str.empty())
		{
			m_object = cJSON_Parse(str.c_str());
			m_document = std::shared_ptr<JsonDocument>(new JsonDocument(m_object));
			checkParseError();
			}
	}

private:
	void checkParseError()
	{
		const char* err = cJSON_GetErrorPtr();
		if (NULL != err)
		{
			std::stringstream sb;
			sb << "JSON Parsing error:"
			   << std::endl
			   << err
			   << std::endl;

			std::string errorMessage(sb.str());
			// Log
			throw std::runtime_error(errorMessage);
		}
	}

private:
	JsonObject* m_object; // TODO: should be const?
	std::shared_ptr<JsonDocument> m_document;
};

#define checkObject(obj, key) checkObjectT(obj, key, false, __FUNCTION__);
#define checkObjectKey(obj, key) checkObjectT(obj, key, true, __FUNCTION__);

#define checkArray(obj, index) checkArrayT(obj, index, __FUNCTION__);

template<typename EX, typename T>
void JSONGenericObject::JSONThrow(const JSONGenericObject& obj, const char* paramName, const T& paramVal, const std::string& msg, const char*const fct)
{
	std::stringstream sstr;
	sstr << "Function: '" << fct << "' " << std::endl;
	sstr << "msg: '" << msg << "' " << std::endl;
	sstr << "param: " << paramName << "='" << paramVal << "' " << std::endl;
	sstr << "param: JSON: '"; obj.stringify(sstr); sstr << "' " << std::endl;

	throw EX(sstr.str());
}

inline void JSONGenericObject::checkObjectT(const JSONGenericObject& obj, const std::string& key, bool checkKey, const char*const fct)
{
	if (obj.m_jsonDocument->getJsonArray())
		JSONThrow<std::logic_error>(obj, "key", key, "not available for an array", fct);
	/*
	// Commented to allow obj.getObject("key1").getObject("key2") when key1 don't exist
	if (!obj.m_jsonDocument->getJsonObject())
		JSONThrow<std::logic_error>(obj, "key", key, "current object is NULL", fct);
	if (checkKey && !cJSON_GetObjectItemCaseSensitive(obj.m_jsonDocument->getJsonObject(), key.c_str()))
		JSONThrow<std::invalid_argument>(obj, "key", key, "key not present", fct);
	*/
}

inline void JSONGenericObject::checkArrayT(const JSONGenericObject& obj, size_t index, const char*const fct)
{
	if (obj.m_jsonDocument->getJsonObject())
		JSONThrow<std::logic_error>(obj, "index", index, "not available for an object", fct);
	/*
	// Commented to allow obj.getObject("key1").getArray("key2") when key1 don't exist
	if (!obj.m_jsonDocument->getJsonArray())
		JSONThrow<std::logic_error>(obj, "index", index, "current array is NULL", fct);
	if (index >= cJSON_GetArraySize(obj.m_jsonDocument->getJsonArray()))
		JSONThrow<std::invalid_argument>(obj, "index", index, "out of bound", fct);
	*/
}

JSONGenericObject::JSONGenericObject(const JSONGenericObject & other)
	: m_jsonDocument(other.m_jsonDocument)
{
}

JSONGenericObject & JSONGenericObject::operator=(const JSONGenericObject & rhs)
{
	m_jsonDocument = rhs.m_jsonDocument;
	return *this;
}

JSONGenericObject::JSONGenericObject()
{
}

JSONGenericObject::JSONGenericObject(std::shared_ptr<JsonDocumentWrapper> document)
	: m_jsonDocument(document)
{
}

JSONGenericObject::~JSONGenericObject()
{
}

void JSONGenericObject::stringify(std::ostream & ostream) const
{
	if (!m_jsonDocument)
		return;
	m_jsonDocument->stringify(ostream);
}

JSONGenericObject::operator bool() const
{
	return m_jsonDocument && m_jsonDocument->Object() != NULL;
}

const JSONGenericObject JSONGenericObject::getObject(const std::string & key) const
{
	//checkObject(*this, key);

	JsonObject* jsonObject = cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
	if(!jsonObject)
		return JSONGenericObject();
	if (!cJSON_IsObject(jsonObject))
		return JSONGenericObject();
	return JSONGenericObject(std::shared_ptr<JsonDocumentWrapper>(new JsonDocumentWrapper(jsonObject)));
}

bool JSONGenericObject::haveValue(const std::string & key) const
{
	checkObject(*this, key);
	return NULL != cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
}

bool JSONGenericObject::isDouble(const std::string & key) const
{
	checkObject(*this, key);
	const JsonObject* jsonObject = cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
	return cJSON_IsNumber(jsonObject);
}

bool JSONGenericObject::isString(const std::string & key) const
{
	checkObject(*this, key);
	const JsonObject* jsonObject = cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
	return cJSON_IsString(jsonObject);
}

bool JSONGenericObject::isBool(const std::string & key) const
{
	checkObject(*this, key);
	const JsonObject* jsonObject = cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
	return cJSON_IsBool(jsonObject);
}

bool JSONGenericObject::isInteger(const std::string & key) const
{
	checkObject(*this, key);
	const JsonObject* jsonObject = cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
	if (! cJSON_IsNumber(jsonObject))
		return false;

	const double value = cJSON_GetNumberValue(jsonObject);
	double intpart;
	return (std::modf(value, &intpart) == 0.0);
}

bool JSONGenericObject::isNull(const std::string & key) const
{
	checkObject(*this, key);
	const JsonObject* jsonObject = cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
	return cJSON_IsNull(jsonObject);
}

double JSONGenericObject::getDouble(const std::string & key) const
{
	if (!isDouble(key))
		JSONThrow<std::invalid_argument>(*this, "key", key, "bad datatype", __FUNCTION__);
	checkObjectKey(*this, key);
	const JsonObject* jsonObject = cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
	return cJSON_GetNumberValue(jsonObject);
}

int64_t JSONGenericObject::getInteger(const std::string & key) const
{
	if (!isInteger(key))
		JSONThrow<std::invalid_argument>(*this, "key", key, "bad datatype", __FUNCTION__);
	checkObjectKey(*this, key);

	const JsonObject* jsonObject = cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
	const double value = cJSON_GetNumberValue(jsonObject);
	return static_cast<int64_t>(value);
}

std::string JSONGenericObject::getString(const std::string & key) const
{
	if (!isString(key))
		JSONThrow<std::invalid_argument>(*this, "key", key, "bad datatype", __FUNCTION__);
	checkObjectKey(*this, key);
	const JsonObject* jsonObject = cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
	return std::string(cJSON_GetStringValue(jsonObject));
}

bool JSONGenericObject::getBool(const std::string & key) const
{
	if (!isBool(key))
		JSONThrow<std::invalid_argument>(*this, "key", key, "bad datatype", __FUNCTION__);
	checkObjectKey(*this, key);
	const JsonObject* jsonObject = cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
	if (cJSON_IsTrue(jsonObject))
		return true;
	if (cJSON_IsFalse(jsonObject))
		return false;
	return false;
}

const JSONGenericObject JSONGenericObject::getArray(const std::string & key) const
{
	//checkObject(*this, key);
	JsonObject* jsonObject = cJSON_GetObjectItemCaseSensitive(m_jsonDocument->Object(), key.c_str());
	if (! cJSON_IsArray(jsonObject))
		return JSONGenericObject();
	return JSONGenericObject(std::shared_ptr<JsonDocumentWrapper>(new JsonDocumentWrapper(jsonObject)));
}

size_t JSONGenericObject::size() const
{
	if (!m_jsonDocument->getJsonArray())
		throw std::logic_error("JSONGenericObject::size() is only avalible for an array");
	return cJSON_GetArraySize(m_jsonDocument->getJsonArray());
}

const JSONGenericObject JSONGenericObject::operator[](size_t index) const
{
	checkArray(*this, index);
	JsonObject* jsonObject = cJSON_GetArrayItem(m_jsonDocument->Object(), index);
	return JSONGenericObject(std::shared_ptr<JsonDocumentWrapper>(new JsonDocumentWrapper(jsonObject)));
}

bool JSONGenericObject::isDouble(size_t index) const
{
	checkArray(*this, index);
	JsonObject* jsonObject = cJSON_GetArrayItem(m_jsonDocument->Object(), index);
	return cJSON_IsNumber(jsonObject);
}

bool JSONGenericObject::isString(size_t index) const
{
	checkArray(*this, index);
	JsonObject* jsonObject = cJSON_GetArrayItem(m_jsonDocument->Object(), index);
	return cJSON_IsString(jsonObject);
}

bool JSONGenericObject::isBool(size_t index) const
{
	checkArray(*this, index);
	JsonObject* jsonObject = cJSON_GetArrayItem(m_jsonDocument->Object(), index);
	return cJSON_IsBool(jsonObject);
}

bool JSONGenericObject::isInteger(size_t index) const
{
	checkArray(*this, index);
	JsonObject* jsonObject = cJSON_GetArrayItem(m_jsonDocument->Object(), index);
	if (! cJSON_IsNumber(jsonObject))
		return false;

	const double value = cJSON_GetNumberValue(jsonObject);
	double intpart;
	return (std::modf(value, &intpart) == 0.0);
}

bool JSONGenericObject::isNull(size_t index) const
{
	checkArray(*this, index);
	JsonObject* jsonObject = cJSON_GetArrayItem(m_jsonDocument->Object(), index);
	return cJSON_IsNull(jsonObject);
}

double JSONGenericObject::getDouble(size_t index) const
{
	checkArray(*this, index);
	if (!isDouble(index))
		JSONThrow<std::invalid_argument>(*this, "index", index, "bad datatype", __FUNCTION__);
	JsonObject* jsonObject = cJSON_GetArrayItem(m_jsonDocument->Object(), index);
	return cJSON_GetNumberValue(jsonObject);
}

std::string JSONGenericObject::getString(size_t index) const
{
	checkArray(*this, index);
	if (!isString(index))
		JSONThrow<std::invalid_argument>(*this, "index", index, "bad datatype", __FUNCTION__);
	JsonObject* jsonObject = cJSON_GetArrayItem(m_jsonDocument->Object(), index);
	return std::string(cJSON_GetStringValue(jsonObject));
}

bool JSONGenericObject::getBool(size_t index) const
{
	checkArray(*this, index);
	if (!isBool(index))
		JSONThrow<std::invalid_argument>(*this, "index", index, "bad datatype", __FUNCTION__);
	JsonObject* jsonObject = cJSON_GetArrayItem(m_jsonDocument->Object(), index);
	if (cJSON_IsTrue(jsonObject))
		return true;
	if (cJSON_IsFalse(jsonObject))
		return false;
	return false;
}

int64_t JSONGenericObject::getInteger(size_t index) const
{
	checkArray(*this, index);
	if (!isInteger(index))
		JSONThrow<std::invalid_argument>(*this, "index", index, "bad datatype", __FUNCTION__);
	JsonObject* jsonObject = cJSON_GetArrayItem(m_jsonDocument->Object(), index);
	const double value = cJSON_GetNumberValue(jsonObject);
	return static_cast<int64_t>(value);
}

JSONReader::JSONReader(const JSONReader & other)
	: m_jsonDocument(other.m_jsonDocument)
{
}

JSONReader::JSONReader()
	: m_jsonDocument(new JsonDocumentWrapper())
{
}

JSONReader & JSONReader::operator=(const JSONReader & rhs)
{
	m_jsonDocument = rhs.m_jsonDocument;
	return *this;
}

JSONReader::~JSONReader()
{
}

const JSONGenericObject JSONReader::parse(std::istream & stream)
{
	m_jsonDocument->parse(stream);
	return JSONGenericObject(m_jsonDocument);
}

// wasm: no boost
// const JSONGenericObject JSONReader::parse(const boost::filesystem::path & file)
// {
// 	const boost::filesystem::path path = boost::filesystem::canonical(file);
// 	std::ifstream ifstr(path.string().c_str());
// 	JSONGenericObject root = parse(ifstr);
// 	ifstr.close();
//
// 	return root;
// }

const JSONGenericObject JSONReader::parse(const std::string & str)
{
	m_jsonDocument->parse(str);
	return JSONGenericObject(m_jsonDocument);
}

void JSONReader::stringify(std::ostream & ostream) const
{
	m_jsonDocument->stringify(ostream);
}
