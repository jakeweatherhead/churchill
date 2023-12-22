#ifndef DERIBIT_FUTURES_OBJECT_H
#define DERIBIT_FUTURES_OBJECT_H

#include <string>

class DeribitFuturesObject
{

public:
  double mid_price;
  double estimated_delivery_price;
  double volume_notional;
  double volume_usd;
  double quote_currency;
  double creation_timestamp;
  std::string base_currency;
  double volume;
  double open_interest;
  double ask_price;
  double bid_price;
  double price_change;
  double mark_price;
  std::string instrument_name;
  double last;
  double low;
  double high;

};

#endif // DERIBIT_FUTURES_OBJECT_H