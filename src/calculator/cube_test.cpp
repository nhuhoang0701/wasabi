#include "cube.h"

#include <dbproxy/dbproxy.h>

#include <test_tools/TestAssert.h>

int main()
{
	TEST_INIT();
	
	using namespace cube;
	Cube cube;
	cube.addDim(Cube::eAxe::Column, Object("Dim0"));
	cube.addDim(Cube::eAxe::Row, Object("Dim1"));
	cube.addMeas("Meas0");
	cube.addMeas("Meas1");
	
	CPPUNIT_ASSERT_EQUAL(cube.getBody().size(),0);
	
	dbproxy::Row row;
	row.push_back(dbproxy::Value("val0"));
	row.push_back(dbproxy::Value("val1"));
	row.push_back(dbproxy::Value("val2"));
	row.push_back(dbproxy::Value("val3"));
	
	cube.insertRow(row);
	CPPUNIT_ASSERT_EQUAL(cube.getBody().size(),1);
	CPPUNIT_ASSERT_EQUAL(cube.getBody()[0].size(),4);
	
	return TEST_HAVEERROR();
}
