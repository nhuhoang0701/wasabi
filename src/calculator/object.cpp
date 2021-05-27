#define WASABI_NOLOG
#include "object.h"

#include "cube.h"

#include "InA_query_model/Function.h"
#include "InA_query_model/Selection/Selection.h"

#include <InA_query_model/QueryEx.h>

#include "storage.h"

#include <stdexcept>

#include <common/Log.h>
#include <string>


namespace calculator
{
	Object::Object(const std::string& name)
		: m_name(name)
	{
	}
	
	Object::Object(const Object& other)
	{
		m_name = other.m_name;
		m_datastorage = other.m_datastorage;
		m_formula = other.m_formula;
		m_selection = other.m_selection;
		m_restrictedObject = other.m_restrictedObject;
		m_dataColumn = other.m_dataColumn;
	}
	Object& Object::operator=(const Object& rhs)
	{
		m_name = rhs.m_name;
		m_datastorage = rhs.m_datastorage;
		m_formula = rhs.m_formula;
		m_selection = rhs.m_selection;
		m_restrictedObject = rhs.m_restrictedObject;
		m_dataColumn = rhs.m_dataColumn;
		return *this;
	}
	bool Object::operator==(const Object& rhs) const
	{
		if(m_name != rhs.m_name)
			return false;
		if(m_datastorage != rhs.m_datastorage)
			return false;
		if(m_formula != rhs.m_formula)
			return false;
		if(m_restrictedObject != rhs.m_restrictedObject)
			return false;
		if(m_dataColumn != rhs.m_dataColumn)
			return false;
		return m_selection == rhs.m_selection;
	}

	void Object::materialyze(const Cube& cube)
	{
		ScopeLog sc("Object::materialyze");
		Logger::log("name", getName());
		Logger::log("m_restrictedObject", m_restrictedObject!=nullptr);
		Logger::log("m_formula", m_formula!=nullptr);
		Logger::log("m_dataColumn", m_dataColumn!=nullptr);
		Logger::log("m_datastorage", m_datastorage!=nullptr);
		Logger::log("m_selection", m_selection!=nullptr);
		m_datastorage = &cube.getStorage();
		if(m_formula == nullptr && m_selection == nullptr)
		{
			m_dataColumn = m_datastorage->getColumn(m_name);
			if(!m_datastorage->haveCol(getName()))
				throw std::runtime_error("Object name '" + getName() + "' not found in datastorage");
		}
		else
		{
			if(m_datastorage->haveCol(getName()))
				throw std::runtime_error("Object name '" + getName() + "' already use in datastorage");	
		}
		
		if(m_restrictedObject != nullptr)
			m_restrictedObject->materialyze(cube);
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

	

	typedef std::tuple<size_t, const DataStorage*> Context;
	void getValueCallback2(const void* context, const std::string& name, common::Value& value)
	{
		const DataStorage* data = std::get<1>(*static_cast<const Context*>(context));
		const size_t row = std::get<0>(*static_cast<const Context*>(context));

		Logger::debug("getValueCallback2 row:", row);
		Logger::debug("getValueCallback2 name:", name);
		Logger::debug("getValueCallback2 data:", data==nullptr);
		Logger::debug("getValueCallback2 data->haveCol(name):", data==nullptr?false:data->haveCol(name));
		value = data->getColumn(name)->getValueAtRowIdx(row);
	}
	common::Value Object::aggregate(const ina::query_model::Selection* selection) const
	{
		ScopeLog sc("Object::aggregate_all");
		const common::eDataType datatype = m_dataColumn->getDataType();

		common::Value value;
		//TODO: Add value exception in common::Value
		if(datatype == common::eDataType::Numeric)
			value = std::nan("0");
		else
			value = "##NULL##";
		Logger::error("Local agregation, NYI hardcoded to sum");

		if(datatype == common::eDataType::Numeric)
		{
			double sum = 0;
			for(size_t rowIndex = 0; rowIndex < m_dataColumn->getRowCount(); rowIndex++)
			{
				if(selection == nullptr)
					sum +=  std::get<double>(m_dataColumn->getValueAtRowIdx(rowIndex));
				else
				{
					Context ctxt(rowIndex, m_datastorage);
					if(ina::query_model::eval(&ctxt, selection, getValueCallback2))
					{
						sum +=  std::get<double>(m_dataColumn->getValueAtRowIdx(rowIndex));
					}
				}
			}
			value = sum;
		}
		else
		{
			value = "#MULTIVALUE";
		}
		return value;
	}
	common::Value Object::aggregate(const indexisSet& indexes, const ina::query_model::Selection* selection) const
	{
		ScopeLog sc("Object::aggregate");
		Logger::log("name", getName());
		Logger::log("indexes.size()", indexes.size());
		if(indexes.isAll())
			return aggregate(selection);

		const common::eDataType datatype = m_dataColumn->getDataType();
		common::Value value;
		//TODO: Add value exception in common::Value
		if(datatype == common::eDataType::Numeric)
		{
			Logger::log("datatype", "Numeric");
			value = std::nan("0");
		}
		else
		{
			Logger::log("datatype", "String");
			value = "##NULL##";
		}

		if(indexes.size() == 1)
		{
			if(selection == nullptr)
				value = m_dataColumn->getValueAtRowIdx(indexes[0]);
			else
			{
				Context ctxt(indexes[0], m_datastorage);
				if(ina::query_model::eval(&ctxt, selection, getValueCallback2))
				{
					value = m_dataColumn->getValueAtRowIdx(indexes[0]);
				}
			}
		}
		else if(indexes.size() > 1 )
		{
			Logger::error("Local agregation, NYI hardcoded to sum");
			if(datatype == common::eDataType::Numeric)
			{
				double sum = 0;
				for(const auto& rowIndex : indexes)
				{
					if(selection == nullptr)
						sum +=  std::get<double>(m_dataColumn->getValueAtRowIdx(rowIndex));
					else
					{
						Context ctxt(rowIndex, m_datastorage);
						if(ina::query_model::eval(&ctxt, selection, getValueCallback2))
						{
							sum +=  std::get<double>(m_dataColumn->getValueAtRowIdx(rowIndex));
						}
					}
				}
				value = sum;
			}
			else
			{
				value = "#MULTIVALUE"; // TODO: Add vlaue exception in common::Value
			}
		}
		return value;
	}

} // namespace calculator
