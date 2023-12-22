#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

#include <string>
#include <jansson.h>

class JsonProcessor
{
public:
  static std::string formatJSON(const std::string &jsonStr);
};

#endif // JSON_PROCESSOR_H
