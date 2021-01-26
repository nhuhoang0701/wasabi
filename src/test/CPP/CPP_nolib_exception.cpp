#include <iostream> 

#define EXPORT __declspec(dllexport)

#include <vector>         // For vector

const char* str_VECTOR_DOUBLE_d(double d)
{
	std::vector<double> v;
	std::vector<int> vint;
	double da = 1984;
	
	if(v.empty() == false)
		return "Size of the vector should be 0";
	
	v.push_back(d);
	if(v.size() != 1)
		return "Size of the vector should be 1";
	if(v[0] != 1984)
		return "vector[0] should be 1984";

	return "Test OK";
}

/*
//TODO: this case need std::terminate for the moment not clear where std::terminate should be implemented in the JS kernel side
class Ae
{
public:
	Ae(int i){_i = i;}
	~Ae(){_i = 0;}

	int get() const {return _i;}
private:
	int _i;
};

EXPORT int i_TrycatchObjDesc_i(int i)
{
	int ie = 1234567890;
	try
	{
		throw Ae(i);
	}
	catch(const Ae& e)
	{
		ie = e.get();
	}
	catch(...)
	{
		ie = -1;
	}

	return ie;
}
*/

EXPORT int i_TRYCATCHTHROW_v()
{
	try
	{
		try
		{
			throw 'e';
		}
		catch(const char e)
		{
			if(e != 'e')
				return 1;
			throw 1;
		}
	}
	catch(const int e)
	{
		if(e != 1)
			return 2;
		return 0;
	}
	return -1;
}

EXPORT void v_JUSTTHROW_v()
{
		throw;
}

EXPORT const char* str_TRYTHROWINTANDCATCHIT_v()
{
	int i = 42;
	try
	{
		throw i;
		return "KO: exception not throw";
	}
	catch(const int& i)
	{
		if(i == 42)
			return "Test OK";
		return "KO: catch by value of exception";
	}
	catch(...)
	{
		return "KO: int type exception not catched";
	}

	return "KO: exception not catched";
}

EXPORT int i_TRYCATCHDIV_i(int d)
{
	int i = 1;
	try
	{
		i = i / d;
	}
	catch(...)
	{
		return -1;
	}

	return i;
}

EXPORT double d_TRYCATCHDIV_d(double d)
{
	double i = 1;
	try
	{
		i /= d;
	}
	catch(...)
	{
		return -1;
	}

	return i;
}

int main() {
	std::cout << "Call d_TRYCATCHDIV_d \n";
	int res = d_TRYCATCHDIV_d(5);
	std::cout << "val d_TRYCATCHDIV_d: " << res << "\n\n";
	if (res != 0)
		return res;
	
	std::cout << "Call i_TRYCATCHDIV_i \n";
	res = i_TRYCATCHDIV_i(5);
	std::cout << "val i_TRYCATCHDIV_i: " << res << "\n\n";
	if (res != 0)
		return res;
	
	
	std::cout << "Call str_VECTOR_DOUBLE_d \n";
	const char* pRes = str_VECTOR_DOUBLE_d(1984);
	std::cout << "val str_VECTOR_DOUBLE_d: " << pRes << "\n";
	if (strcmp( pRes, "Test OK") == 0)
		;
	else
	{
		std::cout << "Error str_VECTOR_DOUBLE_d: " << pRes << "\n";
		return -1;
	}
	
	
	std::cout << "Call str_TRYTHROWINTANDCATCHIT_v \n";
	std::cout << "Compilation Not supported str_TRYTHROWINTANDCATCHIT_v \n\n";
	//const char* c_res = str_TRYTHROWINTANDCATCHIT_v();
	/*std::cout << "val str_TRYTHROWINTANDCATCHIT_v: " << c_res << "\n";
	if (strcmp( c_res, "KO: exception not catched"))
		return -1;
	*/
	std::cout << "Call v_JUSTTHROW_v \n";
	std::cout << "Compilation Not supported v_JUSTTHROW_v \n\n";
	//v_JUSTTHROW_v();
	
	std::cout << "Call i_TRYCATCHTHROW_v \n";
	std::cout << "Compilation Not supported i_TRYCATCHTHROW_v \n\n";
	//res = i_TRYCATCHTHROW_v();

	return 0;
}

