#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

#include <string>
#include <jansson.h>

class JsonProcessor
{
public:
    static std::string formatJSON(const std::string &jsonStr);
    static double getNumber(json_t *value, const char *key);
    static int getInteger(json_t *value, const char *key);
    static std::string getString(json_t *value, const char *key);
};

#endif // JSON_PROCESSOR_H
