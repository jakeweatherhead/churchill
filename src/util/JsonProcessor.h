#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

#include "exchange/DeltaOption.h"

#include "OptionPair.h"

#include <string>
#include <jansson.h>
#include <nlohmann/json.hpp>

class JsonProcessor
{
public:
    static std::string formatJSON(const std::string &jsonStr);
    static double parseNumber(const json_t *value, const std::string fieldName);
    static int parseInteger(const json_t *value);
    static std::string parseString(const json_t *value);
    static std::string convertOptionPairsToString(const std::vector<OptionPair> &candidates);
    static std::string convertDeltaOptionsToString(const std::vector<DeltaOption> &deltaOptions);
};

#endif // JSON_PROCESSOR_H
