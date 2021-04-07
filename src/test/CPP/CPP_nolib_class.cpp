#include <iostream> 
#include <test_tools/TestAssert.h>

#define EXPORT 


/***************************************
Class define for testing purpose
*/


class iType
{
public:
	virtual const char* getType()= 0;
};
class TypeA : public iType
{
public:
	virtual const char* getType() {return "TypeA";};
};
class TypeB : public iType
{
public:
	virtual const char* getType() {return "TypeB";};
};


class A
{
public:
	A(char c, char* staticStr, unsigned char& index) : m_char(c), m_staticStr(staticStr), m_index(index)
	{
		getStaticStr()[m_index++] = ' ';
		getStaticStr()[m_index++] = getVal();
		getStaticStr()[m_index++] = ':';
		getStaticStr()[m_index++] = 'A';
		getStaticStr()[m_index] = '\0';
	}
	virtual ~A()
	{
		getStaticStr()[m_index++] = ' ';
		getStaticStr()[m_index++] = getVal();
		getStaticStr()[m_index++] = ':';
		getStaticStr()[m_index++] = '~';
		getStaticStr()[m_index++] = 'A';
		getStaticStr()[m_index] = '\0';
	}

protected:
	char  getVal() const {return m_char;}
	char* getStaticStr() const {return m_staticStr;}

private:
	char* m_staticStr;
	char m_char;
protected:
	unsigned char& m_index;
};

class B : public A
{
public:
	B(char i, char* staticStr, unsigned char& index) : A(i, staticStr, index)
	{
		getStaticStr()[m_index++] = ' ';
		getStaticStr()[m_index++] = getVal();
		getStaticStr()[m_index++] = ':';
		getStaticStr()[m_index++] = 'B';
		getStaticStr()[m_index] = '\0';
	}
	virtual ~B()
	{
		getStaticStr()[m_index++] = ' ';
		getStaticStr()[m_index++] = getVal();
		getStaticStr()[m_index++] = ':';
		getStaticStr()[m_index++] = '~';
		getStaticStr()[m_index++] = 'B';
		getStaticStr()[m_index] = '\0';
	}
};

/***************************************
Tests
*/
//Verify construtor destructor order
EXPORT const char* str_DESTRUCTOR_v()
{
	static char str256[256];
	str256[0]= '\0';
	unsigned char index = 0;
	A a0('A', str256, index);
	A a1('a', str256, index);

	return str256;
}

//Verify construtor v destructor order

EXPORT const char* str_VDESTRUCTOR_v()
{
	static char str256[256];
	str256[0]= '\0';
	unsigned char index = 0;
	{
		str256[index++] = '0';
		A a('A', str256, index);
		str256[index++] = ' ';
		str256[index++] = '1';
		{
			str256[index++] = ' ';
			str256[index++] = '2';
			B b('B', str256, index);
			str256[index++] = ' ';
			str256[index++] = '3';
		}
		str256[index++] = ' ';
		str256[index++] = '4';
	}
	str256[index++] = ' ';
	str256[index++] = '5';
	str256[index] = '\0';

	// Should be "0 A:A 1 2 B:A B:B 3 B:~B B:~A 4 A:~A 5"
	return str256;
}

EXPORT const char* str_CALL_VIRTUAL_METHOD_c(char c)
{
	TypeA typeA;
	TypeB typeB;
	iType* type = 0;
	if(c == 'a')
		type = &typeA;
	else if(c == 'b')
		type = &typeB;
	
	return type->getType();
}

EXPORT int i_DYNCATS_v()
{
	char str256[256];
	str256[0]= '\0';
	unsigned char index = 0;
	int i1;
	A a(i1, str256, index);
	B b(i1, str256, index);

	A* apb = &b;

	if( dynamic_cast<B*>(apb) == 0)
		return -1;

	if( dynamic_cast<B*>(&a) != 0)
		return -2;

	return 0;
}

int main()
{
	TEST_INIT();

	CPPUNIT_ASSERT_EQUAL(i_DYNCATS_v(),0);
	
	CPPUNIT_ASSERT_EQUAL_STR(str_CALL_VIRTUAL_METHOD_c('a'),"TypeA");
	
	CPPUNIT_ASSERT_EQUAL_STR(str_CALL_VIRTUAL_METHOD_c('b'),"TypeB");
	
	CPPUNIT_ASSERT_EQUAL_STR(str_DESTRUCTOR_v()," A:A a:A a:~A A:~A");
	
	CPPUNIT_ASSERT_EQUAL_STR(str_VDESTRUCTOR_v(),"0 A:A 1 2 B:A B:B 3 B:~B B:~A 4 A:~A 5");

	return TEST_HAVEERROR();
}
