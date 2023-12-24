#ifndef OPTION_PROCESSOR_H
#define OPTION_PROCESSOR_H

#include "OptionPair.h"

#include <string>
#include <vector>

class OptionProcessor
{
public:
    OptionProcessor();
    ~OptionProcessor();
    void createOptionPairs(const std::string &symbol, std::vector<DeribitOption> &deribitOptions, std::vector<DeltaOption> &deltaCallOptions, std::vector<DeltaOption> &deltaPutOptions, std::vector<OptionPair> &candidates);
};

#endif // OPTION_PROCESSOR_H