#pragma once

#include <string>
#include <vector>

namespace dbproxy {class Row;};

namespace cube
{
	class Cube
	{
	public:
	typedef std::vector<std::vector<std::string>> Body;

		Cube();

		const Body&  getBody() const {return m_body;};
		
		void addColumnDim(const std::string& name);
		void addColumnMeas(const std::string& name);

		void insertRow(const dbproxy::Row& row);

	private:
		std::vector<std::string /*name*/>  m_columns;
		
		Body   m_body;
	};
} // cube
