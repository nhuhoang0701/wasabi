#include <iostream> 
#include "CPP_libcxx.hpp"

int main()
{
	std::cout << "Call d_STDSQRT_d \n";
	int res = d_STDSQRT_d(4);
	std::cout << "val d_STDSQRT_d: " << res << "\n\n";
	if (res != 2)
		return -1;
	
	int* pRes = NULL;
	std::cout << "Call pi_NEW_INT_v \n";
	pRes = pi_NEW_INT_v();
	std::cout << "val pi_NEW_INT_v: " << pRes << "\n\n";
	if (pRes == NULL)
		return -1;

	std::cout << "Call v_DELETE_pv \n";
	v_DELETE_pv(pRes);

	return 0;
}


