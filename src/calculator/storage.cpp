#include "storage.h"
#include "common/Log.h"
#include <stdexcept>


namespace calculator
{
	size_t DataStorage::getColIndex(const std::string& col_name) const
	{
		ScopeDebug sc("DataStorage::getColIndex");
		Logger::debug("name", col_name);
		auto iter = m_colsIdxByName.find(col_name);
		if(iter == m_colsIdxByName.end())
			throw std::runtime_error("Column not find");
		return iter->second;
	}
} // cube
