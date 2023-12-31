#include "util/JsonProcessor.h"
#include <iostream>

std::string JsonProcessor::formatJSON(const std::string &jsonStr)
{
    std::string formatted;
    json_error_t error;
    json_t *root = json_loads(jsonStr.c_str(), 0, &error);

    if (!root)
    {
        std::cerr << "Error parsing JSON: " << error.text << std::endl;
        return formatted;
    }

    char *formatted_raw = json_dumps(root, JSON_INDENT(4));
    if (formatted_raw)
    {
        formatted = formatted_raw;
        free(formatted_raw);
    }
    else
    {
        std::cerr << "Error printing JSON" << std::endl;
    }

    json_decref(root);
    return formatted;
}

double JsonProcessor::parseNumber(const json_t *value, const std::string fieldName)
{
    if (!value || json_is_null(value))
    {
        return 0.0;
    }
    else if (!json_is_number(value))
    {
        throw std::runtime_error("json_t value " + fieldName + " is not a number\n");
    }
    return json_number_value(value);
}

int JsonProcessor::parseInteger(const json_t *value)
{
    if (!value || json_is_null(value))
    {
        return 0;
    }
    else if (!json_is_integer(value))
    {
        throw std::runtime_error("json_t value to be parsed to integer is not an integer");
    }
    return json_integer_value(value);
}

std::string JsonProcessor::parseString(const json_t *value)
{
    if (!value || json_is_null(value))
    {
        return "null";
    }
    else if (!json_is_string(value))
    {
        throw std::runtime_error("json_t value to be parsed to string is not a string");
    }
    return json_string_value(value);
}

std::string JsonProcessor::convertOptionPairsToString(const std::vector<OptionPair> &candidates)
{
    nlohmann::ordered_json j;
    for (const auto &candidate : candidates)
    {
        nlohmann::ordered_json jCandidate;
        jCandidate["deltaSymbol"] = candidate.deltaSymbol;
        jCandidate["deribitSymbol"] = candidate.deribitSymbol;
        jCandidate["callPrice"] = candidate.callPrice;
        jCandidate["callCurrency"] = candidate.callCurrency;
        jCandidate["makerFee"] = candidate.makerFee;
        jCandidate["strike"] = candidate.strike;
        jCandidate["putPrice"] = candidate.putPrice;
        jCandidate["putCurrency"] = candidate.putCurrency;
        jCandidate["takerFee"] = candidate.takerFee;
        jCandidate["deribitFuturesMidPrice"] = candidate.deribitFuturesMidPrice;
        jCandidate["capitalRequired"] = candidate.capitalRequired;
        jCandidate["profit"] = candidate.profit;
        jCandidate["returnPerc"] = candidate.returnPerc;
        j.push_back(jCandidate);
    }
    return j.dump();
}

std::string JsonProcessor::convertDeltaOptionsToString(const std::vector<DeltaOption> &options) {
    nlohmann::json j;
    for (const auto &option : options) {
        nlohmann::json jOption;
        jOption["close"] = option.close;
        jOption["contract_type"] = option.contract_type;
        jOption["greeks"] = {
            {"delta", option.greeks.delta},
            {"gamma", option.greeks.gamma},
            {"rho", option.greeks.rho},
            {"theta", option.greeks.theta},
            {"vega", option.greeks.vega}
        };
        jOption["high"] = option.high;
        jOption["low"] = option.low;
        jOption["mark_price"] = option.mark_price;
        jOption["mark_vol"] = option.mark_vol;
        jOption["oi"] = option.oi;
        jOption["oi_change_usd_6h"] = option.oi_change_usd_6h;
        jOption["oi_contracts"] = option.oi_contracts;
        jOption["oi_value"] = option.oi_value;
        jOption["oi_value_symbol"] = option.oi_value_symbol;
        jOption["oi_value_usd"] = option.oi_value_usd;
        jOption["open"] = option.open;
        jOption["price_band"] = {
            {"lower_limit", option.price_band.lower_limit},
            {"upper_limit", option.price_band.upper_limit}
        };
        jOption["product_id"] = option.product_id;
        jOption["quotes"] = {
            {"ask_iv", option.quotes.ask_iv},
            {"ask_size", option.quotes.ask_size},
            {"best_ask", option.quotes.best_ask},
            {"best_bid", option.quotes.best_bid},
            {"bid_iv", option.quotes.bid_iv},
            {"bid_size", option.quotes.bid_size},
            {"impact_mid_price", option.quotes.impact_mid_price},
            {"mark_iv", option.quotes.mark_iv}
        };
        jOption["size"] = option.size;
        jOption["spot_price"] = option.spot_price;
        jOption["strike_price"] = option.strike_price;
        jOption["symbol"] = option.symbol;
        jOption["timestamp"] = option.timestamp;
        jOption["turnover"] = option.turnover;
        jOption["turnover_symbol"] = option.turnover_symbol;
        jOption["turnover_usd"] = option.turnover_usd;
        jOption["volume"] = option.volume;

        j.push_back(jOption);
    }
    return j.dump();
}