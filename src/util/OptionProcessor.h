#ifndef OPTION_PROCESSOR_H
#define OPTION_PROCESSOR_H

#include "OptionPair.h"

#include "exchange/DeribitOption.h"
#include "exchange/DeltaOption.h"
#include "exchange/DeribitFutures.h"

#include <string>
#include <vector>

class OptionProcessor
{
public:
    OptionProcessor();
    ~OptionProcessor();
    std::vector<OptionPair> createOptionPairs(
        const std::string &symbol,
        std::vector<DeribitOption> &deribitOptions,
        std::vector<DeltaOption> &deltaOptions,
        std::vector<DeribitFutures> &deribitFutures);
    DeribitFutures findDeribitFutures(const std::vector<DeribitFutures> &deribitFutures, const std::string &futuresInstrumentName);
};

#endif // OPTION_PROCESSOR_H