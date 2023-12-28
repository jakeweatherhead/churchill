#include "util/JsonProcessor.h"
#include <iostream>

std::string JsonProcessor::formatJSON(const std::string &jsonStr)
{
    std::string formatted;
    json_error_t error;
    json_t *root = json_loads(jsonStr.c_str(), 0, &error);

    if (!root)
    {
        std::cerr << "Error parsing JSON: " << error.text << std::endl;
        return formatted;
    }

    char *formatted_raw = json_dumps(root, JSON_INDENT(4));
    if (formatted_raw)
    {
        formatted = formatted_raw;
        free(formatted_raw);
    }
    else
    {
        std::cerr << "Error printing JSON" << std::endl;
    }

    json_decref(root);
    return formatted;
}

double JsonProcessor::parseNumber(json_t *value, std::string fieldName)
{
    if (!value || json_is_null(value))
    {
        return 0.0;
    }
    else if (!json_is_number(value))
    {
        throw std::runtime_error("json_t value " + fieldName + " is not a number\n");
    }
    return json_number_value(value);
}

int JsonProcessor::parseInteger(json_t *value)
{
    if (!value || json_is_null(value))
    {
        return 0;
    }
    else if (!json_is_integer(value))
    {
        throw std::runtime_error("json_t value to be parsed to integer is not an integer");
    }
    return json_integer_value(value);
}

std::string JsonProcessor::parseString(json_t *value)
{
    if (!value || json_is_null(value))
    {
        return "null";
    }
    else if (!json_is_string(value))
    {
        throw std::runtime_error("json_t value to be parsed to string is not a string");
    }
    return json_string_value(value);
}

std::string JsonProcessor::convertOptionPairsToString(const std::vector<OptionPair> &candidates)
{
    nlohmann::json j;
    for (const auto &candidate : candidates)
    {
        nlohmann::json jCandidate;
        jCandidate["deltaSymbol"] = candidate.deribitSymbol;
        jCandidate["deribitSymbol"] = candidate.deribitSymbol;
        jCandidate["callPrice"] = candidate.callPrice;
        jCandidate["makerFee"] = candidate.makerFee;
        jCandidate["strike"] = candidate.strike;
        jCandidate["putPrice"] = candidate.putPrice;
        jCandidate["takerFee"] = candidate.takerFee;
        jCandidate["deribitFuturesMidPrice"] = candidate.deribitFuturesMidPrice;
        jCandidate["capitalRequired"] = candidate.capitalRequired;
        jCandidate["profit"] = candidate.profit;
        jCandidate["returnPerc"] = candidate.returnPerc;
        j.push_back(jCandidate);
    }
    return j.dump();
}