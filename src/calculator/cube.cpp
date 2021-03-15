#include "cube.h"

#include <dbproxy/dbproxy.h>
#include <stdexcept>

namespace calculator
{
	Object::Object(const std::string& name)
		: m_name(name)
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
	
	void Cube::setStorage(std::shared_ptr<const DataStorage> data)
	{
		m_data = data;
	}
} // cube
