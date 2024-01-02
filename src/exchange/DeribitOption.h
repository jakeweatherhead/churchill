#ifndef DERIBIT_OPTION_H
#define DERIBIT_OPTION_H

#include <string>

class DeribitOption
{
public:
  double volume_usd;
  double volume;
  double underlying_price;
  std::string underlying_index;
  std::string quote_currency;
  double price_change;
  double open_interest;
  double mid_price;
  double mark_price;
  double low;
  double last;
  double interest_rate;
  std::string instrument_name;
  double high;
  double estimated_delivery_price;
  long long creation_timestamp; // Assuming timestamp is in UNIX epoch format
  double bid_price;
  std::string base_currency;
  double ask_price;
};

#endif // DERIBIT_OPTION_H