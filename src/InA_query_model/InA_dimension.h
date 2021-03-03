#pragma once
#include <string>
#include <vector>
#include "InA_member.h"
namespace query_model
{
	class InA_dimension
	{
		public:			
			InA_dimension(const std::string & name, const std::string & axename);
			
			const std::string & getName() const;
			const std::string & getAxeName() const;

			const std::vector<InA_member> & getMembers() const;
			void                            addMember(const InA_member & member);

		private:
			InA_dimension();
			std::string _name; 
			std::string _axename; 

			std::vector<InA_member> _members;
	};
}