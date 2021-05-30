//#define WASABI_NOLOG

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

	void Object::materialyze(const Cube& cube)
	{
		ScopeLog sc("Object::materialyze");
		Logger::log("name", getName());
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
	//TODO: Perf. use index instead name
	void getValueCallback2(const void* context, const std::string& name, common::Value& value)
	{
		const DataStorage* data = std::get<1>(*static_cast<const Context*>(context));
		const size_t row = std::get<0>(*static_cast<const Context*>(context));

		Logger::debug("getValueCallback2 row:", row);
		Logger::debug("getValueCallback2 name:", name);
		Logger::debug("getValueCallback2 data!=nullptr:", data!=nullptr);
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
				ScopeLog sc(std::to_string(rowIndex));
				if(selection == nullptr)
					sum +=  m_dataColumn->getValueAtRowIdx(rowIndex).getDouble();
				else
				{
					Context ctxt(rowIndex, m_datastorage);
					if(ina::query_model::eval(&ctxt, selection, getValueCallback2))
					{
						sum +=  m_dataColumn->getValueAtRowIdx(rowIndex).getDouble();
					}
				}
			}
			value = sum;
		}
		else
		{
			value = "#MULTIVALUE";
		}
		Logger::log("value", value);
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
			ScopeLog sc(std::to_string(indexes[0]));
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
					ScopeLog sc(std::to_string(rowIndex));
					if(selection == nullptr)
						sum +=  m_dataColumn->getValueAtRowIdx(rowIndex).getDouble();
					else
					{
						Context ctxt(rowIndex, m_datastorage);
						if(ina::query_model::eval(&ctxt, selection, getValueCallback2))
						{
							sum +=  m_dataColumn->getValueAtRowIdx(rowIndex).getDouble();
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
		Logger::log("value", value);
		return value;
	}

} // namespace calculator
