#include "QueryEx.h"

#include "Function.h"
#include "Parameter.h"

#include <cmath> // For std::pow
#include <iostream>
#include <tuple>

namespace ina::query_model {        
	const std::string QueryEx::DIMENSION_OF_MEASURES_NAME = "CustomDimension1";

	common::Value eval(const void* context, const ina::query_model::Parameter& param, void (*getValueCallback)(const void* context, const std::string& nameObj, common::Value& value));
	size_t getDeps(const ina::query_model::Parameter& param, std::vector<std::string>& deps);

	common::Value eval(const void* context, const ina::query_model::Function& fct, void (*getValueCallback)(const void* context, const std::string& nameObj, common::Value& value));
	size_t getDeps(const ina::query_model::Function& fct, std::vector<std::string>& deps);

     QueryEx::QueryEx(const Definition& queryDefinition)
     : m_queryDefinition(queryDefinition)
    {
    }
     QueryEx::QueryEx(const Definition& queryDefinition, const ina::metadata::Cube& dsCube)
     : m_queryDefinition(queryDefinition), m_dsCube(&dsCube)
    {
    }
    
    void QueryEx::getResultObjects(std::vector<std::tuple<std::string /*name*/, std::string/*aggregation*/, common::eDataType>>& resultObjects) const
    {
        for (const auto& dimension : getQueryDefinition().getDimensions())
        {
            // Add members
            for(const auto& member : getVisibleMembers(dimension) )
            {
                const std::string& memberName = ina::query_model::Member::getName(member);
                if(isDataSourceObject(memberName)==false)
                    continue;

                std::string agg = member.getAggregation();
                if(agg.empty())
                {
                    std::string msg;
                    msg += "WASABI: ERROR: Missing aggreation in the InA request for member '" + memberName + "' hardcoded SUM will be used, NYI metadata from SQL tables";
                    std::cerr << msg;
                    agg = "SUM";
                }
                resultObjects.push_back(std::make_tuple(memberName, agg, common::eDataType::Numeric)); // TODO: Add datatype to Member
            }
            // From formula
            for(const auto& member : getVisibleMembers(dimension) )
            {
                if(member.getFormula() == nullptr)
                    continue;

                std::vector<std::string> deps;
                ina::query_model::getDeps(*member.getFormula(), deps);

                for(const auto&memberName : deps )
                {
                    if(isDataSourceObject(memberName)==false)
                        continue;

                    bool conatainsIt = false;
                    for(const auto& select : resultObjects)
                    {
                        if(std::get<0>(select)==memberName)
                        {
                            conatainsIt=true;
                            continue;
                        }
                    }
                    if(conatainsIt)
                        continue;

                    std::string agg = member.getAggregation();
                    if(agg.empty())
                    {
                        std::string msg;
                        msg += "WASABI: ERROR: Missing aggreation in the InA request for member '" + memberName + "' hardcoded SUM will be used, TODO: Read it from metadata";
                        std::cerr << msg;
                        agg = "SUM";
                    }
                    resultObjects.push_back(std::make_tuple(memberName, agg, common::eDataType::Numeric)); // TODO: datatype
                }
            }
            //From attributes
            for(const auto& attribut : dimension.getAttributes())
            {
                if(isDataSourceObject(attribut.getName())==false)
                    continue;
                resultObjects.push_back(std::make_tuple(attribut.getName(), "", common::eDataType::String));
            }
        }
    }

    const ina::query_model::Definition& QueryEx::getQueryDefinition() const
    {
        return m_queryDefinition;
    }
    const ina::metadata::Cube* QueryEx::getDSCube() const
    {
        return m_dsCube;
    }

    bool QueryEx::isDataSourceObject(const std::string& attributeName) const
    {
        //TODO: Should be in ina::metadata::Cube but some tests didn't have a Cube intance for the moment
        if(attributeName=="[Measures].[Measures]" || attributeName=="[Measures].[Name]")
            return false;

        if(m_dsCube!=nullptr)
            return m_dsCube->containsDataSourceColumn(attributeName);
        
        return true;// Should not append expect for test
    }

    void traverse(std::vector<Element>& filters, const SelectionElement& selectionElement)
    {
        if (selectionElement.getType() == SelectionElement::Type::Operator) 
        {
            if (!selectionElement.getSubSelections().empty())
            {
                for(const auto& childSelectionElement : selectionElement.getSubSelections()) 
                {
                    traverse(filters, childSelectionElement);
                }
            }
        }
        else if (selectionElement.getType() == SelectionElement::Type::SetOperand)
        {
            if (!selectionElement.getElements().empty())
            {
                filters.insert(std::end(filters), std::begin(selectionElement.getElements()), std::end(selectionElement.getElements()));
            }
        }
    }

    const std::vector<Member> QueryEx::getVisibleMembers(const Dimension& dimension) const 
    {
        std::vector<Element> filters;
        traverse(filters, m_queryDefinition.getSelection().getOperator());
        if (!filters.empty())
        {
            std::vector<Member> visibleMembers;
            for(const auto& member : dimension.getMembers())
            {
                for(const auto& filter : filters)
                {
                    /* 
                    MDS_TheDefinitveGuide_2_1 page 237
                    5.4.1 FieldName
                        The name of the field, which is used in the comparison. Even though the term “FieldName”
                        was use, only attribute names may be specified in this field.
                        Even some virtual attributes (e.g. hierarchy key or description) can be used.
                    */
                    bool matchFieldName = false;                    
                    for(const auto& attribute : dimension.getAttributes())
                    {
                        if (filter.getFieldName()  == attribute.getName())
                        {
                            matchFieldName = true;
                            break;
                        }
                    }
                    if (matchFieldName)
                    {
                        if (filter.getComparisonOperator() == Element::ComparisonOperator::EqualTo )
                        {
                            if(filter.getLowValue() == ina::query_model::Member::getName(member))
                                visibleMembers.push_back(member);
                        }
                        else
                            throw std::runtime_error("Only EqualTo operator implemented for member selection");
                    }
                }
            }
            // If some meber have been select it return it , else go to the default behavior are return all memeners
            if(!visibleMembers.empty())
                return visibleMembers;
        }

        return dimension.getMembers();
    }
    
	common::Value eval(const void* context, const ina::query_model::Parameter& param, void (*getValueCallback)(const void* context, const std::string& nameObj, common::Value& value))
    {
        common::Value paramValue;
        switch (param.getType())
        {
            case ina::query_model::Parameter::eConstant:
            {
                if(param.getValueType()=="String")
                    paramValue = std::stod(param.getValue());
                else
                    paramValue = std::stod(param.getValue());
                break;
            }
            case ina::query_model::Parameter::eFunction:
            {
                paramValue = eval(context, param.getFunction(), getValueCallback);
                break;
            }
            case ina::query_model::Parameter::eMember:
            {
                if(getValueCallback == nullptr)
                    throw std::runtime_error("Missing callback to evaluate Member");
                getValueCallback(context, param.getName(), paramValue);
                break;
            }
            case ina::query_model::Parameter::eUndefined:
                throw std::runtime_error("eUndefined parameter type");
            default:
                throw std::runtime_error("Unkown parameter type");
        }

        return paramValue;
    }

	size_t getDeps(const ina::query_model::Parameter& param, std::vector<std::string>& deps)
    {
        size_t nbOfDeps = 0;
        switch (param.getType())
        {
            case ina::query_model::Parameter::eConstant:
                break;
            case ina::query_model::Parameter::eFunction:
            {
                nbOfDeps += getDeps(param.getFunction(), deps);
                break;
            }
            case ina::query_model::Parameter::eMember:
            {
                deps.push_back(param.getName());
                nbOfDeps++;
                break;
            }
            default:
                throw std::runtime_error("Unkonw function parameter type");
        }

        return nbOfDeps;
    }
    
    double getDouble(const common::Value& val)
    {
        if(std::holds_alternative<double>(val))
        {
            return std::get<double>(val);
        }
        if(std::holds_alternative<std::string>(val))
        {
            return std::stod(std::get<std::string>(val));
        }
        throw std::runtime_error("getDouble: Not a double");
    }

    common::Value eval(const void* context, const ina::query_model::Function& fct, void (*getValueCallback)(const void* context, const std::string& nameObj, common::Value& value))
    {
        std::vector<common::Value>  paramValues(fct.getParameterCount());
        for(size_t i = 0; i < fct.getParameterCount(); i++)
        {
            const ina::query_model::Parameter& param = fct.getParameter(i);
            paramValues[i] = eval(context, param, getValueCallback);
        }

        switch(fct.getFunctionType())
        {
            case ina::query_model::Function::eAdd:
            {
                return getDouble(paramValues[0]) + getDouble(paramValues[1]);
            }
            case ina::query_model::Function::eSubtract:
            {
                return getDouble(paramValues[0]) - getDouble(paramValues[1]);
            }
            case ina::query_model::Function::eMultipy:
            {
                return getDouble(paramValues[0]) * getDouble(paramValues[1]);
            }
            case ina::query_model::Function::eDivide:
            {
                return getDouble(paramValues[0]) / getDouble(paramValues[1]);
            }
            case ina::query_model::Function::eSquare:
            {
                return std::pow(getDouble(paramValues[0]), getDouble(paramValues[1]));
            }
            case ina::query_model::Function::eDecFloat:
            {
                return std::stod(std::get<std::string>(paramValues[0]));
            }
            default:
                throw std::runtime_error("Not yet implemented function parameter type");

        }
        return 42.0;
    }

    size_t getDeps(const ina::query_model::Function& fct, std::vector<std::string>& deps)
    {
        size_t nbOfDeps = 0;
        for(size_t i = 0; i < fct.getParameterCount(); i++)
        {
            const ina::query_model::Parameter& param = fct.getParameter(i);
            switch (param.getType())
            {
                case ina::query_model::Parameter::eConstant:
                    continue;
                case ina::query_model::Parameter::eFunction:
                {
                    nbOfDeps += getDeps(param.getFunction(), deps);
                    break;
                }
                case ina::query_model::Parameter::eMember:
                {
                    deps.push_back(param.getName());
                    nbOfDeps++;
                    break;
                }
                default:
                    throw std::runtime_error("Unkonw function parameter type");
            }
        }

        return nbOfDeps;
    }
    
    common::Value eval(const void* context, const ina::query_model::Formula& formula, void (*getValueCallback)(const void* context, const std::string& nameObj, common::Value& value))
    {
        return eval(context, formula.getParameter(), getValueCallback);
    }
    size_t getDeps(const ina::query_model::Formula& formula, std::vector<std::string>& deps)
    {
        return getDeps(formula.getParameter(), deps);
    }

    
    size_t getDeps(const ina::query_model::Selection& selection, std::vector<std::string>& deps)
    {
        throw std::runtime_error("NYI Selection dependencies");
    }
}