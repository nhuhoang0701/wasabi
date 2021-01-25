#include <wasm/wasm_helper.h>

const char* str = "STATIC_STRING_FROM_LIB_DLA";
EXPORT const char* str_GETSTATICSTRING()
{
	return str;
}
