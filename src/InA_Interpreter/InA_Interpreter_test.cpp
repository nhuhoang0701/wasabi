#include "InA_Interpreter.h"

#include <iostream>

#include "json/jsonReader.h"
#include "test_data.h"

#define WASM_EXPORT extern "C"

int main()
{
	std::cout << "main executed:"
              << __FILE__ << ':'
              << __LINE__ << '\n';
	return 0;
}

