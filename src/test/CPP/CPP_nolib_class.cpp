#include <iostream> 
#define EXPORT __declspec(dllexport)


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
	}
	virtual ~A()
	{
		getStaticStr()[m_index++] = ' ';
		getStaticStr()[m_index++] = getVal();
		getStaticStr()[m_index++] = ':';
		getStaticStr()[m_index++] = '~';
		getStaticStr()[m_index++] = 'A';
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
	}
	virtual ~B()
	{
		getStaticStr()[m_index++] = ' ';
		getStaticStr()[m_index++] = getVal();
		getStaticStr()[m_index++] = ':';
		getStaticStr()[m_index++] = '~';
		getStaticStr()[m_index++] = 'B';
	}
};

/***************************************
Tests
*/
//Verify construtor destructor order
EXPORT const char* str_DESTRUCTOR_v()
{
	char* str256 = new char[256];
	str256[0]= '\0';
	unsigned char index = 0;
	A a0('A', str256, index);
	A a1('a', str256, index);

	return str256;
}

//Verify construtor v destructor order

EXPORT const char* str_VDESTRUCTOR_v()
{
	char* str256 = new char[256];
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
	char* str256 = new char[256];
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

int main() {
	int i = i_DYNCATS_v();
	if (i != 0)
		return i;

	std::cout << "Call virtual method a\n";
	const char *resA = str_CALL_VIRTUAL_METHOD_c('a');
	std::cout << "val a: " << resA << "\n\n";
	if (strcmp( resA, "TypeA"))
		return -1;
	
	std::cout << "Call virtual method b\n";
	const char *resB = str_CALL_VIRTUAL_METHOD_c('b');
	std::cout << "val b: " << resB << "\n\n";
	if (strcmp( resB, "TypeB"))
		return -1;
	
	std::cout << "Call destructor \n";
	const char* resDestuctor = str_DESTRUCTOR_v();
	std::cout << "val destructor: " << resDestuctor << "\n\n";
	if (strcmp( resDestuctor, " A:A a:A a:~A A:~A"))
		return -1;
		
	std::cout << "Call virtual destructor \n";
	const char* resVDestuctor = str_VDESTRUCTOR_v();
	std::cout << "val Vdestructor: " << resVDestuctor << "\n\n";
	if (strcmp( resVDestuctor, "0 A:A 1 2 B:A B:B 3 B:~B B:~A 4 A:~A 5"))
		return -1;

	return 0;
}
