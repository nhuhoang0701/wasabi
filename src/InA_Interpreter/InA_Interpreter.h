
// Don't delete the returned char *
// Not threat safe: so calls all theses methods in the same theard or at least in a sequantial way.
#include <stdint.h>

// these function will call this methoed to provide the resylt.
// void ina_callback_response(int32_t ID, const char* action, const char* response)

extern "C" void void_getServerInfo_int32(int32_t ID);
extern "C" void void_getResponse_int32_json(int32_t ID, const char* InA);

// Should be define in webassembly side or in JavaScrpt side to received the InA response
extern "C" void ina_callback_response(int32_t ID, const char* action, const char* response);
