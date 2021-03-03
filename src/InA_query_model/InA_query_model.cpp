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
                InA_dimension dimension(name, InA_dimension::Type::ObjectsDimension, datatype);
                addDimension(dimension);
            }
            else
            {
                InA_dimension dimension(name, InA_dimension::Type::MeasuresDimension, datatype);
                // FIXME: aggregation are set on members, not the dimension itself
                InA_member member(name, datatype, aggregation);
                dimension.addMember(member);
                addDimension(dimension);
            }            
        }
    }

    void InA_query_model::addDimension(const InA_dimension& dimension)
    {
        m_objs.push_back(dimension);
    }

    void InA_query_model::write(JSONWriter& writer) const
    {
        JSON_MAP(writer);
        writer.pair("table", getTable());
        writer.key("objects");
        JSON_LIST(writer);
        for(const query_model::InA_dimension & object : getObjects())
        {
            JSON_MAP(writer);
            std::string aggregation;
            switch(object.getType())
			{
				case query_model::InA_dimension::Type::ObjectsDimension:
				case query_model::InA_dimension::Type::VariableDimension:
				{
					aggregation = NO_AGG;
				}
				break;
				case query_model::InA_dimension::Type::MeasuresDimension:
				{
                    const InA_member member = *(object.getMembers().begin());
					aggregation = member.getAggregation();
				}
				break;
				default:
				break;
			}
            writer.pair("name", object.getName());
            writer.pair("datatype", object.getDatatype());
            writer.pair("aggregation", aggregation);
        }
    }

    std::ostream& operator<<(std::ostream& os, const InA_query_model& queryModel)
    {
        JSONWriter writer(os);
        queryModel.write(writer);
        return os;
    }

} //query_model