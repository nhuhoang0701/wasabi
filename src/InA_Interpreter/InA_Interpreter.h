//WARNINGS: 
//	 Not threat safe: so calls all theses methods in the same theard or at least in a sequantial way.

#include <stdint.h>   // For int32_t

enum eRequestType
{
    eUndefined = -1,
    eLoad = 0,
    eGetServerInfo = 1,
    eGetResponse = 2,
    eSubmitCube = 3
};
static const char* toString(eRequestType type)
{
	switch (type)
	{
	case eLoad:
		return "Load";
	case eGetServerInfo:
		return "GetServerInfo";
	case eGetResponse:
		return "GetResponse";
	case eSubmitCube:
		return "SubmitCube";
	case eUndefined:
		return "Undefined";
	default:
	    return nullptr;
	}
}

/**
 * @name doIt , the result will be provide throw the call bacl @ina_callback_response
 *
 * @param ID Identifier the request (will be propagated to the ina_callback_response)
 * @param type Type of the request (will be propagated to the ina_callback_response)
 * @param inaRequest The request , nullptr for GetServerInfo
 *
 */
extern "C" void doIt(int32_t ID, eRequestType type, const char* inaRequest);

/**
 * @name ina_callback_response , callback which provide the result of doIt function
 *
 * @param ID Identifier the request (will be propagated to the ina_callback_response)
 * @param type Type of the request (will be propagated to the ina_callback_response)
 * @param inaRequest The request , nullptr for GetServerInfo
 *
 */
extern "C" void ina_callback_response(int32_t ID, eRequestType type, const char* inaResponse);
