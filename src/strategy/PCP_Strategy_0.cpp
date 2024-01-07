#include "strategy/PCP_Strategy_0.h"

std::vector<OptionPair> PCP_Strategy_0::filterArbitrageOpportunities(std::vector<OptionPair> &candidates)
{
    std::cout << "Num candidates: " << candidates.size() << std::endl; // TODO: Remove
    std::vector<OptionPair> optionPairs;
    for (OptionPair &optionPair : candidates)
    {
        setCapitalRequired(optionPair);
        setProfit(optionPair);
        setReturnPerc(optionPair);
        if (returnIsSufficient(optionPair) && liquidityIsSufficient(optionPair))
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
    optionPair.profit = optionPair.callPrice;               // Sell call
    optionPair.profit -= optionPair.makerFee;               // Pay maker fee
    optionPair.profit += optionPair.strike;                 // Receive strike
    optionPair.profit -= optionPair.putPrice;               // Buy put
    optionPair.profit -= optionPair.takerFee;               // Pay taker fee
    optionPair.profit -= optionPair.deribitFuturesMidPrice; // Buy futures
}

void PCP_Strategy_0::setReturnPerc(OptionPair &optionPair)
{
    optionPair.returnPerc = optionPair.profit / optionPair.capitalRequired;
}

bool PCP_Strategy_0::returnIsSufficient(OptionPair &optionPair)
{
    return optionPair.returnPerc > 0.006; // Standard bank monthly interest rate
}

bool PCP_Strategy_0::liquidityIsSufficient(OptionPair &optionPair)
{
    return optionPair.callPrice > 0.0 && optionPair.putPrice > 0.0;
}