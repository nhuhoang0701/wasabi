#pragma once
#include <string>
namespace query_model
{
	typedef std::string Datatype;
	typedef std::string Aggregation;
	
	class InA_member
	{
		private:
			InA_member();
			std::string _name; 
			Datatype _datatype;
			Aggregation _aggregation;
		public:
			InA_member(const std::string & name, const Datatype & datatype, const Aggregation & aggregation);
			const std::string& getName() const;
			const Datatype & getDatatype() const;
			const Aggregation & getAggregation() const;
	};
}