#include "ResultSetFeatureRequest.h"

#include <json/jsonReader.h>

#include <string>

namespace ina::query_model
{
    static int64_t getWindowingValue(const JSONGenericObject& json, const std::string& name)
    {
        if (json.isNull(name) == false && json.haveValue(name))
            return json.getInteger(name);

        return -1;
    }

    void Read(ResultSetFeatureRequest& obj, const JSONGenericObject& jsonNode)
    {
        if (JSONGenericObject subSetDescription = jsonNode.getObject("SubSetDescription"))
        {
            obj.m_SubSetDescription.m_ColumnFrom = getWindowingValue(subSetDescription, "ColumnFrom");
            obj.m_SubSetDescription.m_ColumnTo = getWindowingValue(subSetDescription, "ColumnTo");
            obj.m_SubSetDescription.m_RowFrom = getWindowingValue(subSetDescription, "RowFrom");
            obj.m_SubSetDescription.m_RowTo = getWindowingValue(subSetDescription, "RowTo");
        }
    }
}