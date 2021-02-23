#include "InA_query_model.h"

#include <iostream>

namespace query_model
{
    void InA_query_model::addDim(const std::string& name, const Datatype& datatype)
    {
        Object dim = std::make_tuple(name, datatype, NO_AGG);
	    //std::cout << "InA_query_model::addDim " << std::get<0>(dim) << " " << std::get<1>(dim) << " " <<  std::get<2>(dim) << std::endl;
        m_objs.push_back(dim);
    }


	void InA_query_model::addMeas(const std::string& name, const Datatype& datatype, const Aggregation& agg)
    {
        Object meas = std::make_tuple(name, datatype, agg);
	    //std::cout << "InA_query_model::addMeas " << std::get<0>(meas) << " " <<  std::get<1>(meas) << " " <<  std::get<2>(meas) << std::endl;
        m_objs.push_back(meas);
    }

} //query_model