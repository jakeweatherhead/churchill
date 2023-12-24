#ifndef PCP_STRATEGY_0_H
#define PCP_STRATEGY_0_H

#include "OptionPair.h"

#include <vector>

class PCP_Strategy_0
{

public:
    std::vector<OptionPair> filterArbitrageOpportunities(std::vector<OptionPair> &candidates);
    void setCapitalRequired(OptionPair &optionPair);
    void setProfit(OptionPair &optionPair);
    void setReturnPerc(OptionPair &optionPair);
    bool returnIsSufficient(OptionPair &optionPair);

};

#endif // PCP_STRATEGY_0_H