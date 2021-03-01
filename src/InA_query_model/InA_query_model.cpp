#include "InA_query_model.h"

#include <json/jsonReader.h>
#include <json/jsonWriter.h>

#include <iostream>

namespace query_model
{
    InA_query_model::InA_query_model(const JSONGenericObject& serializedQueryModel) : m_table(serializedQueryModel.getString("table"))
    {
        const JSONGenericObject& objects = serializedQueryModel.getArray("objects");
        for(int i = 0; i < objects.size();i++)
        {
            const JSONGenericObject& object = objects[i];
            std::string name = object.getString("name");
            std::string datatype = object.getString("datatype");
            std::string aggregation = object.getString("aggregation");
            if(aggregation.empty())
            {
                addDim(name, datatype);
            }
            else
            {
                addMeas(name, datatype, aggregation);
            }
        }
    }

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

    void InA_query_model::write(JSONWriter& writer) const
    {
        JSON_MAP(writer);
        writer.pair("table", getTable());
        writer.key("objects");
        JSON_LIST(writer);
        for(const Object& object : getObjects())
        {
            JSON_MAP(writer);
            writer.pair("name", std::get<0>(object));
            writer.pair("datatype", std::get<1>(object));
            writer.pair("aggregation", std::get<2>(object));
        }
    }

    std::ostream& operator<<(std::ostream& os, const InA_query_model& queryModel)
    {
        JSONWriter writer(os);
        queryModel.write(writer);
        return os;
    }

} //query_model