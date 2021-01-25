//#include <wasm/wasm_helper.h>

#include <dlfcn.h>        // For fopen and co
#include <stdio.h>         // For printf

#define EXPORT __declspec(dllexport)

const char* str = "STATIC_STRING_FROM_LIB_DL";
EXPORT const char* str_GETSTATICSTRING()
{
	return str;
}

EXPORT void* pv_DLOPEN_str(const char* path)
{
	return dlopen(path, RTLD_GLOBAL | RTLD_NOW);
}

EXPORT void* pv_DLSYM_pv_str(void* handle, const char* symbol)
{
	return dlsym(handle, symbol);
}

EXPORT const char* str_EXECUTE_SYM_pv(void* handle)
{
	const char* (*fct)() = handle;
	return fct();
}

EXPORT int i_DLCLOSE_pv(void* handle)
{
	return dlclose(handle);
}

EXPORT int i_DLOPEN_DLCLOSE_str(void* path)
{
	void* handle = dlopen(path, RTLD_GLOBAL | RTLD_NOW);
	if(handle == 0)
		return 0;
	return dlclose(handle);
}

EXPORT const char* str_DLOPENTHIS_DLSYM_CALLSYMFCT_DLCLOSE_v()
{
	void* handle = dlopen("/wasm_clang/libv_clibc_dl", RTLD_GLOBAL | RTLD_NOW);
	if(handle == 0)
	{
		fprintf(stderr, "str_DLOPENTHIS_DLSYM_CALLSYMFCT_DLCLOSE_v: Error during dlopen\n");
		fflush(stderr);
		return 0;
	}
	printf("str_DLOPENTHIS_DLSYM_CALLSYMFCT_DLCLOSE_v: dlopen return %p \n", handle);
	fflush(stdout);

	const char* (*functionPtr)() = dlsym(handle, "str_GETSTATICSTRING");
	printf("functionPtr %p", functionPtr);
	fflush(stdout);
	if(functionPtr == 0)
	{
		fprintf(stderr, "str_DLOPENTHIS_DLSYM_CALLSYMFCT_DLCLOSE_v: Error during dlsym\n");
		fflush(stderr);
		dlclose(handle);
		return 0;
	}
	const char* res = functionPtr();
	printf("str_DLOPENTHIS_DLSYM_CALLSYMFCT_DLCLOSE_v: str_GETSTATICSTRING return: %s\n", res);
	fflush(stdout);
	dlclose(handle);

	return res;
}

EXPORT const char* str_DLOPEN_DLSYM_CALLSYMFCT_DLCLOSE_v()
{
	void* handle = dlopen("/wasm_clang/libv_clibc_dl_a", RTLD_GLOBAL | RTLD_NOW);
	if(handle == 0)
	{
		fprintf(stderr, "str_DLOPEN_DLSYM_CALLSYMFCT_DLCLOSE_v: Error during dlopen(libv_clibc_dl_a)\n");
		return 0;
	}
	printf("str_DLOPEN_DLSYM_CALLSYMFCT_DLCLOSE_v: dlopen(libv_clibc_dl_a) return %p \n", handle);
	
	const char* (*functionPtr)() = dlsym(handle, "str_GETSTATICSTRING");
	printf("functionPtr %p", functionPtr);
	if(functionPtr == 0)
	{
		fprintf(stderr, "str_DLOPEN_DLSYM_CALLSYMFCT_DLCLOSE_v: Error during dlsym(str_GETSTATICSTRING)\n");
		dlclose(handle);
		return 0;
	}
	const char* res = functionPtr();
	printf("str_DLOPEN_DLSYM_CALLSYMFCT_DLCLOSE_v: str_GETSTATICSTRING return: %s\n", res);
	dlclose(handle);

	return res;
}

EXPORT const char* str_GETSTATICSTRING2()
{
	return str;
}

