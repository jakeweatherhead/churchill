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

double JsonProcessor::parseNumber(json_t *value, const char *key)
{
    json_t *jsonValue = json_object_get(value, key);
    if (!jsonValue || !json_is_number(jsonValue))
    {
        return 0.0;
    }
    return json_number_value(jsonValue);
}

int JsonProcessor::parseInteger(json_t *value, const char *key)
{
    json_t *jsonValue = json_object_get(value, key);
    if (!jsonValue || !json_is_integer(jsonValue))
    {
        return 0;
    }
    return json_integer_value(jsonValue);
}

std::string JsonProcessor::parseString(json_t *value, const char *key)
{
    json_t *jsonValue = json_object_get(value, key);
    if (!jsonValue || !json_is_string(jsonValue))
    {
        return "";
    }
    return json_string_value(jsonValue);
}

std::string JsonProcessor::convertOptionPairsToString(const std::vector<OptionPair>& candidates)
{
    nlohmann::json j;
    for (const auto& candidate : candidates)
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