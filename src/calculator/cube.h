#pragma once

#include "storage.h"
#include <memory>
#include <string>
#include <vector>

namespace dbproxy {class Row;};   // dbproxy/dbprxy.h

namespace calculator
{
	class Object
	{
		public:
		Object() = default;
		Object(const std::string& name);

	private:
		std::string m_name;
	};

	class Cube;
	class Axe : public std::vector<Object>
	{
	public:
		Axe(const Cube& cube) : m_cube(cube) {};
		~Axe() = default;

		size_t getCardinality() const;

	private:
		const Cube& m_cube;
	};

	class Cube
	{
	public:
	typedef std::vector<std::vector<std::string>> Body;

		Cube();
		
		enum class eAxe {Row, Column};
		void         addDim(eAxe eAxe, const Object& obj);
		void         addMeas(const std::string& name);

		void         setStorage(std::shared_ptr<const DataStorage> data);

		const Body&  getBody() const {return m_body;};

	private:
		Axe  m_AxeRows;
		Axe  m_AxesColumns;

		std::vector<std::string>            m_meas;
		std::shared_ptr<const DataStorage>  m_data;

		Body   m_body;
	};
} // cube
