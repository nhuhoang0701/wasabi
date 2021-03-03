#pragma once

#include <string>

namespace query_model
{
	typedef std::string Datatype;
	typedef std::string Aggregation;
	
	class InA_member
	{
		public:
			InA_member(const std::string & name, const Aggregation & aggregation);
			const std::string& getName() const;

			const Aggregation & getAggregation() const;

		private:
			InA_member();
			std::string _name; 

			Aggregation _aggregation;
	};
}