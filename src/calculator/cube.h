#pragma once

#include <common/data.h>

#include "object.h"
#include "axe.h"
#include "body.h"

#include <memory>
#include <string>
#include <vector>
#include <map>


namespace calculator
{
	class DataStorage;

	enum class eAxe {Row, Column};
	class Cube
	{
	public:
		Cube();

		void                setStorage(std::shared_ptr<const DataStorage> data);
		const DataStorage&  getStorage() const;

		bool         contain(const Object& obj) const;

		void         addDim(eAxe eAxe, const Object& obj);
		void         addMeasure(const Object& obj);
		void         addFormula(const Object& obj, const ina::query_model::Formula& formula);
		void         addRestriction(const Object& obj, const ina::query_model::Selection& formula);

		void         materialyze();

		Axe&         getAxe(eAxe axe);
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
