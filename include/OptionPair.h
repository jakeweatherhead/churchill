#ifndef OPTION_PAIR_H
#define OPTION_PAIR_H

#include <string>

class OptionPair
{
public:
    std::string deltaSymbol;
    std::string deribitSymbol;
    double callPrice;
    double makerFee;
    double strike;
    double putPrice;
    double takerFee;
    double deribitFuturesMidPrice;
    double capitalRequired;
    double profit;
    double returnPerc;
};

#endif // OPTION_PAIR_H