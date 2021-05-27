//#define WASABI_NOLOG
#include "QueryEx.h"

#include "Function.h"
#include "InA_query_model/Selection/Selection.h"
#include "InA_query_model/Selection/SelectionElement.h"
#include "Parameter.h"
#include "common/data.h"

#include <common//Log.h>

#include <cmath> // For std::pow
#include <cstdint>
#include <stdexcept>
#include <tuple>

namespace ina::query_model {        
	const std::string QueryEx::DIMENSION_OF_MEASURES_NAME = "CustomDimension1";

    common::Value eval(const void* context, const ina::query_model::Parameter& param, void (*getValueCallback)(const void* context, const std::string& nameObj, common::Value& value));
    size_t        getDeps(const ina::query_model::Parameter& param, std::vector<std::string>& deps);

    common::Value eval(const void* context, const ina::query_model::Function& fct, void (*getValueCallback)(const void* context, const std::string& nameObj, common::Value& value));
    size_t        getDeps(const ina::query_model::Function& fct, std::vector<std::string>& deps);
   
    bool          eval(const void* context, const ina::query_model::SelectionElement& selectionElement, void (*getValueCallback)(const void* context, const std::string& nameObj, common::Value& value));
    void          traverse(std::map<std::string/*field name/attribut name*/,std::vector<Element>>& filters, const SelectionElement& selectionElement);

    bool containsIt(const std::string& name, std::vector<std::tuple<std::string /*name*/, std::string/*aggregation*/, common::eDataType>>& resultObjects)
    {
        bool conatainsIt = false;
        for(const auto& select : resultObjects)
        {
            if(std::get<0>(select)==name)
            {
                return true;
            }
        }
        return false;
    }

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
            // From members
            for(const auto& member : getVisibleMembers(dimension) )
            {
                const std::string& memberName = ina::query_model::Member::getName(member);
                if(isDataSourceObject(memberName)==false)
                    continue;

                std::string agg = member.getAggregation();
                if(agg.empty())
                {
                    Logger::error("No aggreation in the InA request for member(NYI red metadata)", memberName );
                    agg = "SUM";
                }
                resultObjects.push_back(std::make_tuple(memberName, agg, common::eDataType::Numeric)); // TODO: Add datatype to Member
            }
            // From formulas
            for(const auto& member : getVisibleMembers(dimension) )
            {
                if(member.getFormula() == nullptr)
                    continue;

                std::vector<std::string> deps;
                getDeps(*member.getFormula(), deps);

                for(const auto&memberName : deps )
                {
                    if(isDataSourceObject(memberName)==false)
                        continue;

                    if(containsIt(memberName, resultObjects))
                        continue;

                    std::string agg = member.getAggregation();
                    if(agg.empty())
                    {
                        std::string msg;
                        msg += "Missing aggreation in the InA request for member '" + memberName + "' hardcoded SUM will be used, TODO: Read it from metadata\n";
                        Logger::error(msg);
                        agg = "SUM";
                    }
                    resultObjects.push_back(std::make_tuple(memberName, agg, common::eDataType::Numeric)); // TODO: datatype
                }
            }
            // From restriction
            for(const auto& member : getVisibleMembers(dimension) )
            {
                if(member.getSelection() == nullptr)
                    continue;

                std::map<std::string, std::vector<Element>> selector;

                traverse(selector, member.getSelection()->getOperator());
                
                for(const auto& attribut: selector)
                {
                    if(attribut.first != "[Measures].[Measures]")
                    {
                        if(!containsIt(attribut.first, resultObjects))
                            resultObjects.push_back(std::make_tuple(attribut.first, "", common::eDataType::String));
                    }
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

    
    void QueryEx::getGroupBy(std::vector<std::string>& group_by) const
    {
        for (const auto& dimension : getQueryDefinition().getDimensions())
        {
            if( ! ina::query_model::QueryEx::isDimensionOfMeasures(dimension))
            {
                for(const auto& attribut : dimension.getAttributes())
                    group_by.push_back(attribut.getName());
            }
            
            // From restriction
            for(const auto& member : getVisibleMembers(dimension) )
            {
                if(member.getSelection() == nullptr)
                    continue;

                std::map<std::string, std::vector<Element>> selector;

                traverse(selector, member.getSelection()->getOperator());
                
                for(const auto& attribut: selector)
                {
                    if(attribut.first != "[Measures].[Measures]")
                    {
                        if( std::find(group_by.cbegin(), group_by.cend(), attribut.first) == group_by.cend())
                            group_by.push_back(attribut.first);
                    }
                }
            }
        }
    }

    void QueryEx::getSortObjects(std::vector<std::pair<std::string /*name*/, ina::query_model::Sort::Direction>>& order_by) const
    {
        if (!getQueryDefinition().getSorts().empty())
        {
            for(const auto& querySort : getQueryDefinition().getSorts())
            {
                // case of MemberSort, TODO: in the Grid
                if (ina::query_model::QueryEx::DIMENSION_OF_MEASURES_NAME != querySort.getObjectName())
                {
                    const std::string& nameSortedDim = querySort.getObjectName();
                    if(getDSCube() != nullptr)
                    {
                        if(querySort.getSortType()==ina::query_model::Sort::SortType::MemberKey)
                            order_by.push_back(std::make_pair(getDSCube()->getDimension(nameSortedDim).getKeyAttribute().getName(), querySort.getDirection()));
                        else if(querySort.getSortType()==ina::query_model::Sort::SortType::MemberText)
                            order_by.push_back(std::make_pair(getDSCube()->getDimension(nameSortedDim).getTextAttribute().getName(), querySort.getDirection()));
                        else
                            order_by.push_back(std::make_pair(getDSCube()->getDimension(nameSortedDim).getAttributes().at(0).getName(), querySort.getDirection()));
                    }
                    for(const auto& dim : getQueryDefinition().getDimensions())
                    {
                        if(dim.getName() == nameSortedDim)
                        {
                           order_by.push_back(std::make_pair(dim.getAttributes().at(0).getName(), querySort.getDirection()));
                        }
                    }
                }
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

    void traverse(std::map<std::string,std::vector<Element>>& filters, const SelectionElement& selectionElement)
    {
        if (selectionElement.getType() == SelectionElement::Type::Operator) 
        {
            for(const auto& childSelectionElement : selectionElement.getSubSelections()) 
            {
                traverse(filters, childSelectionElement);
            }
        }
        else if (selectionElement.getType() == SelectionElement::Type::SetOperand)
        {
            const std::string& fieldName = selectionElement.getFieldName();
            auto& elements = filters[fieldName];
            elements.insert(std::end(elements), std::begin(selectionElement.getElements()), std::end(selectionElement.getElements()));
        }
    }

    const std::vector<Member> QueryEx::getVisibleMembers(const Dimension& dimension) const 
    {
        std::map<std::string /*fieldName/attributname*/, std::vector<Element>> filters;
        traverse(filters, m_queryDefinition.getSelection().getOperator());
        if (!filters.empty())
        {
            std::vector<Member> visibleMembers;
            for(const auto& member : dimension.getMembers())
            {
                /* 
                MDS_TheDefinitveGuide_2_1 page 237
                5.4.1 FieldName
                    The name of the field, which is used in the comparison. Even though the term “FieldName”
                    was use, only attribute names may be specified in this field.
                    Even some virtual attributes (e.g. hierarchy key or description) can be used.
                */              
                for(const auto& attribute : dimension.getAttributes())
                {
                    if(filters.find(attribute.getName()) != filters.end())
                    {
                        for(const Element& filter : filters[attribute.getName()])
                        {
                            if (filter.getComparisonOperator() == Element::ComparisonOperator::EqualTo )
                            {
                                //TODO: Check getLowValue datatype
                                if(std::get<std::string>(filter.getLowValue()) == ina::query_model::Member::getName(member))
                                    visibleMembers.push_back(member);
                            }
                            else
                                throw std::runtime_error("Only EqualTo operator implemented for member selection");
                        }
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

    bool eval(const void* context, const ina::query_model::Selection* selection, void (*getValueCallback)(const void* context, const std::string& nameObj, common::Value& value))
    {
        if(selection==nullptr)
            return true;
        
        return eval(context, selection->getOperator(), getValueCallback);
    }

    bool eval(const void* context, const ina::query_model::SelectionElement& selectionElement, void (*getValueCallback)(const void* context, const std::string& nameObj, common::Value& value))
    {
        if (selectionElement.getType() == SelectionElement::Type::Operator) 
        {
            ScopeLog sc("eval SelectionElement Operator");
            uint8_t evalRes = 2;
            ina::query_model::LogicalOperator code = selectionElement.getCode();
            for(const auto& childSelectionElement : selectionElement.getSubSelections()) 
            {
                const std::string& fieldName = childSelectionElement.getFieldName();
                ScopeLog sc(fieldName);
                if(fieldName=="[Measures].[Measures]")
                    continue;

                bool res = eval(context, childSelectionElement, getValueCallback);
                Logger::log("res", res);
                if(evalRes == 2)
                    evalRes = res;
                else if(code == ina::query_model::LogicalOperator::Or)
                    evalRes |= res;
                else if(code == ina::query_model::LogicalOperator::And)
                    evalRes &= res;
                else if(code == ina::query_model::LogicalOperator::Not)
                    evalRes = !res;

                Logger::log("evalRes", evalRes);  
            }
            return evalRes;
        }
        else if (selectionElement.getType() == SelectionElement::Type::SetOperand)
        {
            ScopeLog sc("eval SelectionElement SetOperand");
            const std::string& fieldName = selectionElement.getFieldName();
            Logger::log("fieldName", fieldName);
            if(fieldName=="[Measures].[Measures]")
                return false;

            common::Value value;
            getValueCallback(context, fieldName, value);
            Logger::log("value", value);
            for(const auto& element : selectionElement.getElements())
            {
                if(element.getComparisonOperator()==ina::query_model::Element::ComparisonOperator::EqualTo)
                {
                    Logger::log("element.getLowValue()", element.getLowValue());
                    if(element.getLowValue()==value)
                    {
                        Logger::log("match", true);
                        return true;
                    }
                }
                else
                    throw std::runtime_error("Only ComparisonOperator is implemented");
            }
            Logger::log("match", false); 
        }
        return false;
    }

    size_t getDeps(const ina::query_model::Selection& selection, std::vector<std::string>& deps)
    {
        std::map<std::string, std::vector<Element>> selector;

        traverse(selector, selection.getOperator());
        
        const auto& measures = selector["[Measures].[Measures]"];
        for(const auto& measure: measures)
        {
            deps.push_back(std::get<std::string>(measure.getLowValue()));
        }

        return measures.size();
    }
}