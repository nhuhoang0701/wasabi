#include <iostream>       // std::cout
#include <cmath>          // For pow
#include <vector>         // For vector
#include "CPP_libcxx.hpp"
#include "test_tools/TestAssert.h"


EXPORT void v_COUT_i(int i)
{
	std::cout << "Enter in v_print_i, param:" << i << std::endl;
}


EXPORT double d_STDSQRT_d(double d)
{
	return std::sqrt(d);
}


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

	v.reserve(10000);
	if(v.size() != 1)
		return "Size of the vector should be 1";

	v.push_back(d);
	if(v.size() != 2)
		return "Size of the vector should be 2";
	if(v.at(1) != d)
		return "vector[1] bad  be 1984";
	return "Test OK";
}


EXPORT int* pi_NEW_INT_v()
{
	return new int;
}


EXPORT void v_DELETE_pv(void* sz)
{
	delete (int*)sz;

}

