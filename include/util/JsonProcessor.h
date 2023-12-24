#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

#include <string>
#include <jansson.h>
#include <nlohmann/json.hpp>

class JsonProcessor
{
public:
    static std::string formatJSON(const std::string &jsonStr);
    static double parseNumber(json_t *value, const char *key);
    static int parseInteger(json_t *value, const char *key);
    static std::string parseString(json_t *value, const char *key);
    std::string convertToString(const std::vector<OptionPair>& candidates);
};

#endif // JSON_PROCESSOR_H
