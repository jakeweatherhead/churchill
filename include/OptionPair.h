#ifndef OPTION_PAIR_H
#define OPTION_PAIR_H

#include <string>

class OptionPair
{
public:
  int arbID;
  std::string deltaSymbol;
  std::string deribitSymbol;
  double callPrice;
  double makerFee;
  double putPrice;
  double takerFee;
  std::string expiration;
  double strike;
  double deribitFuturesMidPrice;
  double spot;
  double capitalRequired;
  double returnPerc;

  OptionPair();
};

#endif // OPTION_PAIR_H