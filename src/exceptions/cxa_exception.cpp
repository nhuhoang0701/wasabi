#include <iostream>
#include <stdexcept>
#include <typeinfo>


// see: https://code.woboq.org/llvm/libcxxabi/src/cxa_exception.cpp.html

extern "C"
{

void *__cxa_allocate_exception(size_t thrown_size) throw()
{
	std::cerr << "Error: Called unimplemented method: __cxa_allocate_exception" << std::endl;
	return NULL;
}

void __cxa_throw(void *thrown_object, std::type_info *tinfo, void (*dest)(void *))
{
	std::cerr << "Error: Throwing exception!!!" << std::endl;
	if (tinfo && tinfo->name())
	{
		std::cerr << "Exception's type info name: " << tinfo->name() << std::endl;
	}
	else
	{
		std::cerr << "Exception's type: unknown" << std::endl;
	}

	std::cerr << "Attempting to read as std::exception... ";
	std::exception* x = reinterpret_cast<std::exception*>(thrown_object); // this may crash
	std::cerr << std::flush << "Error message: \"" << x->what() << "\"" << std::endl;

	std::cerr << "Exiting program" << std::endl;
	exit(-1);
}


void* __cxa_begin_catch(void* unwind_arg) throw()
{
	std::cerr << "Error: Called unimplemented method: __cxa_begin_catch" << std::endl;
	return unwind_arg;
}

void __cxa_end_catch() throw()
{
	std::cerr << "Error: Called unimplemented method: __cxa_end_catch" << std::endl;
}

}
