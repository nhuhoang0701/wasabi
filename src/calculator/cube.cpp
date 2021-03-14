#include "cube.h"

#include <dbproxy/dbproxy.h>
#include <stdexcept>

namespace cube
{
	Object::Object(const std::string& name)
	{

	}

	void Cube::addDim(eAxe axe, const Object& obj)
	{
		switch (axe)
		{
		case eAxe::Row:
		{
			m_AxeRows.push_back(obj);
			break;
		}
		case eAxe::Column:
		{
			m_AxesColumns.push_back(obj);
			break;
		}
		default:
		throw std::runtime_error("Unknow eAxe");
		}
	}

	void Cube::addMeas(const std::string& name)
	{
		m_meas.push_back(name);
	}
	
	void Cube::insertRow(const dbproxy::Row& row)
	{
		std::vector<std::string> rowBody;
		for(const auto& val : row)
			rowBody.push_back(val.getString());
		m_body.push_back(rowBody);
	}
} // cube
