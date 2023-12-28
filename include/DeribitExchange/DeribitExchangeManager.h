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
    std::string fetchBtcFutures();
    std::string fetchBtcOptions();
    std::string fetchEthFutures();
    std::string fetchEthOptions();
    std::vector<DeribitFutures> parseFuturesToVector(const std::string &jsonStr);
    std::vector<DeribitOption> parseOptionsToVector(const std::string &jsonStr);
    static DeribitOption findDeribitComplement(const std::vector<DeribitOption> &optionsVec, const std::string &optionInstrumentName);
};

#endif // DERIBIT_EXCHANGE_MANAGER_H