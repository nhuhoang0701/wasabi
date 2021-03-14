#pragma once

#include <string>
#include <vector>

namespace dbproxy {class Row;};   // dbproxy/dbprxy.h

namespace cube
{
	class Object
	{
		public:
		Object() = default;
		Object(const std::string& name);
	};

	class Axe : public std::vector<Object>
	{
	public:
		Axe() = default;
		~Axe() = default;

		size_t getCardinality() const;

	private:
	};

	class Cube
	{
	public:
	typedef std::vector<std::vector<std::string>> Body;

		Cube() = default;
		
		enum class eAxe {Row, Column};
		void         addDim(eAxe eAxe, const Object& obj);
		void         addMeas(const std::string& name);

		const Body&  getBody() const {return m_body;};

		void         insertRow(const dbproxy::Row& row);

	private:
		Axe  m_AxeRows;
		Axe  m_AxesColumns;

		std::vector<std::string>  m_meas;
		
		Body   m_body;
	};
} // cube
