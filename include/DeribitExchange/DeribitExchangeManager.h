#ifndef DERIBIT_EXCHANGE_MANAGER_H
#define DERIBIT_EXCHANGE_MANAGER_H

#include <string>

class DeribitExchangeManager
{
public:
  DeribitExchangeManager();
  ~DeribitExchangeManager();
  std::string fetchBTCFutures();
  std::string fetchBTCOptions();
  std::string fetchETHFutures();
  std::string fetchETHOptions();
};

#endif // DERIBIT_EXCHANGE_MANAGER_H