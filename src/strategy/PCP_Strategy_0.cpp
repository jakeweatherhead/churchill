#include "strategy/PCP_Strategy_0.h"

std::vector<OptionPair> PCP_Strategy_0::filterArbitrageOpportunities(std::vector<OptionPair> &candidates)
{
    std::vector<OptionPair> optionPairs;
    for (OptionPair &optionPair : candidates)
    {
        setCapitalRequired(optionPair);
        setProfit(optionPair);
        setReturnPerc(optionPair);
        if (returnIsSufficient(optionPair) && optionPair.callPrice != 0.00 && optionPair.putPrice != 0.0)
        {
            optionPairs.push_back(optionPair);
        }
    }
    return optionPairs;
}

void PCP_Strategy_0::setCapitalRequired(OptionPair &optionPair)
{
    optionPair.capitalRequired = optionPair.callPrice;
    optionPair.capitalRequired += optionPair.makerFee;
    optionPair.capitalRequired += optionPair.takerFee;
    optionPair.capitalRequired += optionPair.deribitFuturesMidPrice;
}

void PCP_Strategy_0::setProfit(OptionPair &optionPair)
{
    optionPair.profit = optionPair.callPrice;
    optionPair.profit -= optionPair.makerFee;
    optionPair.profit += optionPair.strike;
    optionPair.profit -= optionPair.putPrice;
    optionPair.profit -= optionPair.takerFee;
    optionPair.profit -= optionPair.deribitFuturesMidPrice;
}

void PCP_Strategy_0::setReturnPerc(OptionPair &optionPair)
{
    optionPair.returnPerc = optionPair.profit / optionPair.capitalRequired;
}

bool PCP_Strategy_0::returnIsSufficient(OptionPair &optionPair)
{
    return optionPair.returnPerc > 0.01;
}