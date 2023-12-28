#ifndef DERIBIT_FUTURES_H
#define DERIBIT_FUTURES_H

#include <string>

class DeribitFutures
{

public:
    double mid_price;
    double estimated_delivery_price;
    double volume_notional;
    double volume_usd;
    std::string quote_currency;
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

#endif // DERIBIT_FUTURES_H