#include "storage.h"
#include "common/Log.h"
#include <stdexcept>


namespace calculator
{
	size_t DataStorage::getColIndex(const std::string& col_name) const
	{
		Logger::log("name", col_name);
		if(m_colsIdxByName.find(col_name) == m_colsIdxByName.end())
			throw std::runtime_error(col_name);
		return m_colsIdxByName.at(col_name);
	}
} // cube
