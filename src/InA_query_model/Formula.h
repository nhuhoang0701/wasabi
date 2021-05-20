#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "InA_query_model/Function.h"

#include <calculator/common.h>   // For calculator::Value

class JSONGenericObject; // From <json/jsonReader.h>

namespace ina::query_model
{
    class Formula;

	calculator::Value eval(const void* context, const ina::query_model::Formula& fct, void (*getValueCallback)(const void* context, const std::string& nameObj, calculator::Value& value));
	size_t getDeps(const ina::query_model::Formula& fct, std::vector<std::string>& deps);

	void read(Formula& obj, const JSONGenericObject& jsonNode);
	//void write(const Function& obj, JSONWriter& jsonNode);

	class Formula
	{
		public:
			Formula() = default;
			
			const Parameter&  getParameter() const;
			
		private:
			Parameter       m_parameter;

			friend void read(Formula& obj, const JSONGenericObject& jsonNode);
	};
}
