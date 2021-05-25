#include "object.h"

#include "cube.h"

#include "InA_query_model/Function.h"
#include "InA_query_model/Selection/Selection.h"

#include "storage.h"

#include <stdexcept>

#include <iostream>


namespace calculator
{
	Object::Object(const std::string& name)
		: m_name(name)
	{
	}
	
	bool Object::operator==(const Object& rhs) const
	{
		return m_name == rhs.m_name;
	}

	void Object::materialyze(const Cube& cube)
	{
		if(m_formula == nullptr && m_selection == nullptr)
		{
			if(!cube.getStorage().haveCol(getName()))
				throw std::runtime_error("Object name '" + getName() + "' not found in datastorage");

			m_dataColumn = cube.getStorage().getColumn(m_name);
		}
		else
		{
			if(cube.getStorage().haveCol(getName()))
				throw std::runtime_error("Object name '" + getName() + "' already use in datastorage");	
		}
	}

	common::eDataType  Object::getDataType() const
	{
		//TODO: Need getdatatype on the AST
		if(m_formula != nullptr || m_selection != nullptr)
			return common::eDataType::Numeric;

		if(!m_dataColumn)
			throw std::runtime_error("Object: getDataType() no m_dataColumn:" + m_name);
		return m_dataColumn->getDataType();
	}

	size_t Object::getRowCount() const
	{
		if(!m_dataColumn)
			throw std::runtime_error("Object: getRowCount() no m_dataColumn:" + m_name);
		return m_dataColumn->getRowCount();	
	}

	const common::Value& Object::getValueAtRowIdx(size_t rowIndex) const
	{
		if(!m_dataColumn)
			throw std::runtime_error("Object: getValueAtRowIdx() no m_dataColumn:" + m_name);
		return m_dataColumn->getValueAtRowIdx(rowIndex);
	}

	size_t Object::getValueIndexFromRowIdx(size_t rowIndex) const
	{
		if(!m_dataColumn)
			throw std::runtime_error("Object: getValueIndexFromRowIdx() no m_dataColumn:" + m_name);
		return m_dataColumn->getValueIndexFromRowIdx(rowIndex);
	}

	const common::Value& Object::getValueAtValueIdx(size_t valueIndex) const
	{
		if(!m_dataColumn)
			throw std::runtime_error("Object: getValueAtValueIdx() no m_dataColumn:" + m_name);
		return m_dataColumn->getValueAtValueIdx(valueIndex);
	}

	size_t Object::getNumberOfValues() const
	{
		if(!m_dataColumn)
			throw std::runtime_error("Object: getNumberOfValues() no m_dataColumn:" + m_name);
		return m_dataColumn->getNumberOfValues();
	}

	
	common::Value Object::aggregate() const
	{
		common::Value value;
		std::cerr << "WASABI: ERROR: Local agregation, NYI hardcoded to sum" << std::endl;

		const common::eDataType datatype = m_dataColumn->getDataType();
		if(datatype == common::eDataType::Numeric)
		{
			double sum = 0;
			for(size_t i = 0; i < m_dataColumn->getRowCount(); i++)
				sum +=  std::get<double>(m_dataColumn->getValueAtRowIdx(i));
			value = sum;
		}
		else
		{
			value = "#MULTIVALUE";
		}
		return value;
	}

	common::Value Object::aggregate(const indexisSet& indexes) const
	{
		if(indexes.isAll)
			return aggregate();

		common::Value value;
		if(indexes.empty())
		{
			const common::eDataType datatype = m_dataColumn->getDataType();
			if(datatype == common::eDataType::Numeric)
				value = std::nan("0");
			else
				value = "##NULL##";
		}
		else if(indexes.size() == 1)
		{
			value = m_dataColumn->getValueAtRowIdx(indexes.at(0));
		}
		else if(indexes.size() > 1 )
		{
			std::cerr << "WASABI: ERROR: Local agregation, NYI hardcoded to sum" << std::endl;

			const common::eDataType datatype = m_dataColumn->getDataType();
			if(datatype == common::eDataType::Numeric)
			{
				double sum = 0;
				for(const auto& i : indexes)
					sum +=  std::get<double>(m_dataColumn->getValueAtRowIdx(i));
				value = sum;
			}
			else
			{
				value = "#MULTIVALUE";
			}
		}
		return value;
	}

} // namespace calculator
