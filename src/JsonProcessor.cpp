#include "JsonProcessor.h"
#include <iostream>

std::string JsonProcessor::formatJson(const std::string &jsonStr)
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
