#ifndef OPTION_PROCESSOR_H
#define OPTION_PROCESSOR_H

#include "OptionPair.h"

#include <string>
#include <vector>

class OptionProcessor
{
public:
    struct OptionPairHolder
    {
        std::vector<OptionPair> BTCPairs;
        std::vector<OptionPair> ETHPairs;
    };
    OptionProcessor();
    ~OptionProcessor();
    void createOptionPairs(const std::string &symbol, const std::string *deribitOptions, const std::string *deltaCallOptions, const std::string *deltaPutOptions);
};

#endif // OPTION_PROCESSOR_H