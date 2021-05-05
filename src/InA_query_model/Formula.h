#pragma once

#include <string>
#include <vector>
#include <map>
#include "InA_query_model/Function.h"

class JSONGenericObject; // From <json/jsonReader.h>

namespace ina::query_model
{
    //class Function;

    class Formula;

	void read(Formula& obj, const JSONGenericObject& jsonNode);
	//void write(const Function& obj, JSONWriter& jsonNode);

	class Formula
	{
		public:
			
			Formula() = default;

			Formula(const Function& function);
			
			const Function	&getFunction() const;
			
		private:
			
			Function			   m_function;

			friend void read(Formula& obj, const JSONGenericObject& jsonNode);

	};

}
