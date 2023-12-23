#ifndef DELTA_EXCHANGE_MANAGER_H
#define DELTA_EXCHANGE_MANAGER_H

#include "util/RestClient.h"
#include "util/JsonProcessor.h"

#include "DeltaExchange/DeltaOption.h"

#include <string>
#include <fstream>
#include <vector>

class DeltaExchangeManager
{
public:
    RestClient *client;
    DeltaExchangeManager();
    ~DeltaExchangeManager();
    std::string getOptions(std::string optionType, bool writeToFile);
    void parseOptionsToVector(const std::string &currency, const std::string &jsonString, std::vector<DeltaOption> &optionsVec);
};
#endif // DELTA_EXCHANGE_MANAGER_H