#pragma once

#include "storage.h"
#include <memory>
#include <string>
#include <vector>

namespace calculator
{
	class Object
	{
		public:
		Object() = default;
		Object(const std::string& name);

		const std::string& getName() const {return m_name;}

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

	class Body : public std::vector<Object>
	{
		public:
		Body(const Axe& row,const Axe& col);

		size_t  getCellsNbs() const;

		private:
		const Axe& m_axeRow;
		const Axe& m_axeCol;
	};
	class Cube
	{
	public:
		Cube();

		void  setStorage(std::shared_ptr<const DataStorage> data);
		
		enum class eAxe {Row, Column};
		void         addDim(eAxe eAxe, const Object& obj);
		void         addMeas(const std::string& name);

		const Axe&   getAxe(eAxe eAxe) const;
		const Body&  getBody() const {return m_body;};

	private:
		friend class Axe;
		Axe  m_AxeRows;
		Axe  m_AxeColumns;

		std::shared_ptr<const DataStorage>  m_data;

		Body   m_body;
	};
} // cube
