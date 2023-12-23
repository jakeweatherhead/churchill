#ifndef DERIBIT_EXCHANGE_MANAGER_H
#define DERIBIT_EXCHANGE_MANAGER_H

#include "util/RestClient.h"
#include "util/JsonProcessor.h"

#include "DeribitExchange/DeribitFutures.h"
#include "DeribitExchange/DeribitOption.h"

#include <string>
#include <fstream>
#include <vector>

class DeribitExchangeManager
{
public:
    RestClient *client;
    DeribitExchangeManager();
    ~DeribitExchangeManager();
    std::string getBitcoinFutures(bool writeToFile);
    std::string getBitcoinOptions(bool writeToFile);
    std::string getEtherFutures(bool writeToFile);
    std::string getEtherOptions(bool writeToFile);
    void parseFuturesToVector(const std::string &jsonStr, std::vector<DeribitFutures> &futuresVec);
    void parseOptionsToVector(const std::string &jsonString, std::vector<DeribitOption> &optionsVec);
};

#endif // DERIBIT_EXCHANGE_MANAGER_H