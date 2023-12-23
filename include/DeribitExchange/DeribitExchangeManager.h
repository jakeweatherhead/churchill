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
    std::string HTTP_GET_BTC_Futures(bool writeToFile);
    std::string HTTP_GET_BTC_Options(bool writeToFile);
    std::string HTTP_GET_ETH_Futures(bool writeToFile);
    std::string HTTP_GET_ETH_Options(bool writeToFile);
    void parseFuturesToVector(const std::string &jsonStr, std::vector<DeribitFutures> &futuresVec);
    void parseOptionsToVector(const std::string &jsonString, std::vector<DeribitOption> &optionsVec);
};

#endif // DERIBIT_EXCHANGE_MANAGER_H