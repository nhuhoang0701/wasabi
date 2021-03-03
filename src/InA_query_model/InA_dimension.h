#pragma once
#include <string>
#include <vector>
#include "InA_member.h"
namespace query_model
{
	typedef std::string Datatype;
		
	class InA_dimension
	{
		public:
		enum Type
		{
			MeasuresDimension,
			ObjectsDimension,
			VariableDimension
		};
		private:
			InA_dimension();
			std::string _name; 
			Datatype _datatype;
			Type _type;
			std::vector<InA_member> _members;

		public:			
			InA_dimension(const std::string & name, const Type & type, const Datatype & datatype);
			const std::string & getName() const;
			const Datatype & getDatatype() const;
			const InA_dimension::Type & getType() const;
			const std::vector<InA_member> & getMembers() const;
			void addMember(InA_member & member);
	};
}