#include "cube.h"

#include <dbproxy/dbproxy.h>

namespace cube
{
	Cube::Cube()
	{
	}

	void Cube::addColumnDim(const std::string& name)
	{
		m_columns.push_back(name);
	}

	void Cube::addColumnMeas(const std::string& name)
	{
		m_columns.push_back(name);
	}
	
	void Cube::insertRow(const dbproxy::Row& row)
	{
		std::vector<std::string> rowBody;
		for(const auto& val : row)
			rowBody.push_back(val.getString());
		m_body.push_back(rowBody);
	}
} // cube
