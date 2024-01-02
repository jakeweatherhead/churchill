#ifndef OPTION_PAIR_H
#define OPTION_PAIR_H

#include <string>

class OptionPair
{
public:
    std::string deltaSymbol;
    std::string deribitSymbol;
    double callPrice;
    std::string callCurrency;
    double makerFee;
    double strike;
    double putPrice;
    std::string putCurrency;
    double takerFee;
    double deribitFuturesMidPrice;
    double capitalRequired;
    double profit;
    double returnPerc;
};

#endif // OPTION_PAIR_H