#ifndef DELTA_EXCHANGE_MANAGER_H
#define DELTA_EXCHANGE_MANAGER_H

#include "util/RestClient.h"
#include "util/JsonProcessor.h"

#include "DeltaExchange/DeltaOption.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

class DeltaExchangeManager
{
public:
    DeltaExchangeManager();
    ~DeltaExchangeManager();
    std::string fetchOptions(std::string optionType);
    std::vector<DeltaOption> parseOptionsToVector(const std::string &currency, const std::string &jsonStr);
    static std::vector<std::string> split(const std::string &symbol, const char &delimiter);
};
#endif // DELTA_EXCHANGE_MANAGER_H