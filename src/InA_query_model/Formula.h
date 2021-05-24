#pragma once

#include "Parameter.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

class JSONGenericObject; // From <json/jsonReader.h>

namespace ina::query_model
{
    class Formula;

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
