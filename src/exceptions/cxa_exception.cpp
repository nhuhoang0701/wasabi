#include <exception>
#include <iostream>
#include <stdexcept>
#include <typeinfo>


// see: https://code.woboq.org/llvm/libcxxabi/src/cxa_exception.cpp.html

extern "C"
{

void *__cxa_allocate_exception(size_t thrown_size) throw()
{
	void* ptr = malloc(thrown_size);
	if(ptr == nullptr)
		std::terminate();
	//TODO: Need a logger to log error but not in the console
	return ptr;
}

void __cxa_throw(void *thrown_object, std::type_info *tinfo, void (*dest)(void *))
{
	std::string typeinfo =  "unknown";
	std::string msg =  "";
	if (tinfo && tinfo->name())
	{
		typeinfo = tinfo->name();

		if (thrown_object)
		{
			std::exception* x = reinterpret_cast<std::exception*>(thrown_object); // this may crash
			msg = x->what();
		}
	}
	msg = "WASABI:Error: Throwing exception. Exception's type info name: '" + typeinfo + "' message:'" + msg + "'";
	std::cerr << msg << "'";

	std::terminate();
}


void* __cxa_begin_catch(void* unwind_arg) throw()
{
	std::cerr << "WASABI: NYI: __cxa_begin_catch" << std::endl;
	return unwind_arg;
}

void __cxa_end_catch() throw()
{
	std::cerr << "WASABI: NYI: __cxa_end_catch" << std::endl;
}

}
