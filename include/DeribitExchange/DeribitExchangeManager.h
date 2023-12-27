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
    DeribitExchangeManager();
    ~DeribitExchangeManager();
    std::string fetchBtcFutures(bool writeToFile);
    std::string fetchBtcOptions(bool writeToFile);
    std::string fetchEthFutures(bool writeToFile);
    std::string fetchEthOptions(bool writeToFile);
    void parseFuturesToVector(const std::string &jsonStr, std::vector<DeribitFutures> &futuresVec);
    void parseOptionsToVector(const std::string &jsonString, std::vector<DeribitOption> &optionsVec);
    static DeribitOption findDeribitComplement(const std::vector<DeribitOption> &optionsVec, const std::string &optionInstrumentName);
};

#endif // DERIBIT_EXCHANGE_MANAGER_H