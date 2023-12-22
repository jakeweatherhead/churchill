#include "DeribitExchangeManager.h"

DeribitExchangeManager::DeribitExchangeManager()
{
}

DeribitExchangeManager::~DeribitExchangeManager()
{
}

std::string DeribitExchangeManager::fetchBTCFutures()
{
  static const std::string URL = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=BTC&kind=future";
}

std::string DeribitExchangeManager::fetchBTCOptions()
{
  static const std::string URL = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=BTC&kind=option";
}

std::string DeribitExchangeManager::fetchETHFutures()
{
  static const std::string URL = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=ETH&kind=future";
}

std::string DeribitExchangeManager::fetchETHOptions()
{
  static const std::string URL = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=ETH&kind=option";
}
