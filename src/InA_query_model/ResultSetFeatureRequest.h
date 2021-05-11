#pragma once

class JSONGenericObject; // From <json/jsonReader.h>

#include <string>

namespace ina::query_model
{
struct SubSetDescription
{
    int32_t m_ColumnFrom = -1;
    int32_t m_ColumnTo = -1;
    int32_t m_RowFrom = -1;
    int32_t m_RowTo = -1;
};

class ResultSetFeatureRequest;
void Read(ResultSetFeatureRequest& obj, const JSONGenericObject& jsonNode);

class ResultSetFeatureRequest
{
public:
    ResultSetFeatureRequest() = default;

    const SubSetDescription& getSubSetDescription() const;

private:
    friend void Read(ResultSetFeatureRequest& obj, const JSONGenericObject& jsonNode);

    SubSetDescription  m_SubSetDescription;
    std::string m_ResultEncoding;
    std::string m_ResultFormat;
    bool m_UseDefaultAttributeKey = false;
};
}
