#include <iostream>       // std::cout
#include <cmath>          // For pow
#include <vector>         // For vector
#include "CPP_libcxx.hpp"
#include "test_tools/TestAssert.h"

#define EXPORT __declspec(dllexport)

/*
int main() {
   return 0;
}
*/

/*
void *operator new[](std::size_t size) throw(std::bad_alloc)
{
    void* m = malloc(size);
	if(m == 0)
		throw std::bad_alloc();
	return m;
}
*/
/*
void operator delete[](void *p) throw()
{
    free(p);
}
*/
/*
void * operator new(std::size_t size) throw(std::bad_alloc)
{
	void* m = malloc(size);
	if(m == 0)
		throw std::bad_alloc();
	return m;
}
*/

/*
void operator delete(void * p) throw()
{
  free(p);
}
*/


EXPORT void v_COUT_i(int i)
{
	CALL_TEST();
	std::cout << "Enter in v_print_i, param:" << i << std::endl;
}


EXPORT double d_STDSQRT_d(double d)
{
	CALL_TEST();
	return std::sqrt(d);
}

/*
EXPORT const char* str_VECTOR_DOUBLE_d(double d)
{
	std::vector<double> v;
	if(v.empty() == false)
		return "Size of the vector should be 0";
	v.push_back(1984);
	if(v.size() != 1)
		return "Size of the vector should be 1";
	if(v[0] != 1984)
		return "vector[0] should be 1984";

	return "Test OK";
}
*/


EXPORT int* pi_NEW_INT_v()
{
	CALL_TEST();
	return new int;

}


EXPORT void v_DELETE_pv(void* sz)
{
	CALL_TEST();
	//throw "Not implemented";
	delete (int*)sz;

}

