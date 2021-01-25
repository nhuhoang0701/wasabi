#include <wasm/wasm_helper.h>

const char* str = "STATIC_STRING_FROM_LIB_DLB";
EXPORT const char* str_GETSTATICSTRING()
{
	return str;
}

EXPORT const char* str_GETSTATICSTRING_B()
{
	return str;
}
