#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

#include "OptionPair.h"

#include <string>
#include <jansson.h>
#include <nlohmann/json.hpp>
#include "DeltaOption.h"

class JsonProcessor
{
public:
    static std::string formatJSON(const std::string &jsonStr);
    static double parseNumber(json_t *value, std::string fieldName);
    static int parseInteger(json_t *value);
    static std::string parseString(json_t *value);
    static std::string convertOptionPairsToString(const std::vector<OptionPair> &candidates);
    static std::string convertDeltaOptionsToString(const std::vector<DeltaOption> &deltaOptions);
};

#endif // JSON_PROCESSOR_H
