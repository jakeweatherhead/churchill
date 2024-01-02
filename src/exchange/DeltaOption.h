#ifndef DELTA_OPTION_H
#define DELTA_OPTION_H

#include <string>

struct Greeks
{
  std::string delta;
  std::string gamma;
  std::string rho;
  std::string theta;
  std::string vega;
};

struct PriceBand
{
  std::string lower_limit;
  std::string upper_limit;
};

struct Quotes
{
  std::string ask_iv;
  std::string ask_size;
  std::string best_ask;
  std::string best_bid;
  std::string bid_iv;
  std::string bid_size;
  std::string impact_mid_price;
  std::string mark_iv;
};

class DeltaOption
{
public:
  double close;
  std::string contract_type;
  Greeks greeks;
  double high;
  double low;
  std::string mark_price;
  std::string mark_vol;
  std::string oi;
  std::string oi_change_usd_6h;
  std::string oi_contracts;
  std::string oi_value;
  std::string oi_value_symbol;
  std::string oi_value_usd;
  double open;
  PriceBand price_band;
  int product_id;
  Quotes quotes;
  double size;
  std::string spot_price;
  std::string strike_price;
  std::string symbol;
  long long timestamp;
  double turnover;
  std::string turnover_symbol;
  double turnover_usd;
  double volume;
};

#endif // DELTA_OPTION_H