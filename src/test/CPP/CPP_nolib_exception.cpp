#include <iostream> 

#include <test_tools/TestAssert.h>

#define EXPORT 

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

EXPORT int b_TRYCATCHTHROW_v()
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

EXPORT bool b_TRYTHROWINTANDCATCHIT_v()
{
	int i = 42;
	try
	{
		throw i;
		return false;
	}
	catch(const int& i)
	{
		if(i == 42)
			return true;
		return false;
	}
	catch(...)
	{
		return false;
	}

	return false;
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

int main()
{
	TEST_INIT();

	WASABI_CHECK_EQUAL(d_TRYCATCHDIV_d(1),1);
	WASABI_CHECK_EQUAL(d_TRYCATCHDIV_d(0),-1);
		
	WASABI_CHECK_EQUAL(i_TRYCATCHDIV_i(1),1);
	WASABI_CHECK_EQUAL(i_TRYCATCHDIV_i(0),-1);
	
	CPPUNIT_ASSERT(b_TRYTHROWINTANDCATCHIT_v());
	
	WASABI_CHECK_EQUAL(b_TRYCATCHTHROW_v(), 0);
	
	WASABI_CHECK_EQUAL(i_TrycatchObjDesc_i(1977), 1977);

	return TEST_HAVEERROR();
}

