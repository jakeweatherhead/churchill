#include "DeltaExchange/DeltaExchangeManager.h"

#include <iostream>

DeltaExchangeManager::DeltaExchangeManager()
{
}

DeltaExchangeManager::~DeltaExchangeManager()
{
}

std::string DeltaExchangeManager::getOptions(std::string optionType, bool writeData)
{
    static const std::string url = "https://api.delta.exchange/v2/tickers?contract_types=" + optionType;
    std::string res;
    try
    {
        res = RestClient::HTTP_GET(url);
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what();
    }

    std::string formattedRes = JsonProcessor::formatJSON(res);
    if (writeData)
    {
        std::ofstream file;
        file.open("data/delta_" + optionType + ".json");
        file << formattedRes;
        file.close();
    }
    return formattedRes;
}

void DeltaExchangeManager::parseOptionsToVector(const std::string &currency, const std::string &jsonString, std::vector<DeltaOption> &optionsVec)
{
    json_error_t error;
    json_t *root = json_loads(jsonString.c_str(), 0, &error);

    if (!root)
    {
        throw std::runtime_error("Error parsing JSON: " + std::string(error.text));
    }

    json_t *result = json_object_get(root, "result");
    if (!result || !json_is_array(result))
    {
        json_decref(root);
        throw std::runtime_error("Expected 'result' field to be a JSON array");
    }

    size_t index;
    json_t *value;

    json_array_foreach(result, index, value)
    {
        json_t *oi_value_symbolJson = json_object_get(value, "oi_value_symbol");
        std::string optionCurrency = JsonProcessor::parseString(oi_value_symbolJson, "oi_value_symbol");

        if (optionCurrency == currency)
        {
            DeltaOption option;

            option.close = JsonProcessor::parseNumber(value, "close");
            option.contract_type = JsonProcessor::parseString(value, "contract_type");

            json_t *greeksJson = json_object_get(value, "greeks");
            option.greeks.delta = JsonProcessor::parseString(greeksJson, "delta");
            option.greeks.gamma = JsonProcessor::parseString(greeksJson, "gamma");
            option.greeks.rho = JsonProcessor::parseString(greeksJson, "rho");
            option.greeks.theta = JsonProcessor::parseString(greeksJson, "theta");
            option.greeks.vega = JsonProcessor::parseString(greeksJson, "vega");

            option.high = JsonProcessor::parseNumber(value, "high");
            option.low = JsonProcessor::parseNumber(value, "low");
            option.mark_price = JsonProcessor::parseString(value, "mark_price");
            option.mark_vol = JsonProcessor::parseString(value, "mark_vol");
            option.oi = JsonProcessor::parseString(value, "oi");
            option.oi_change_usd_6h = JsonProcessor::parseString(value, "oi_change_usd_6h");
            option.oi_contracts = JsonProcessor::parseString(value, "oi_contracts");
            option.oi_value = JsonProcessor::parseString(value, "oi_value");
            option.oi_value_symbol = JsonProcessor::parseString(value, "oi_value_symbol");
            option.oi_value_usd = JsonProcessor::parseString(value, "oi_value_usd");
            option.open = JsonProcessor::parseNumber(value, "open");

            json_t *priceBandJson = json_object_get(value, "price_band");
            option.price_band.lower_limit = JsonProcessor::parseString(priceBandJson, "lower_limit");
            option.price_band.upper_limit = JsonProcessor::parseString(priceBandJson, "upper_limit");

            json_t *quotesJson = json_object_get(value, "quotes");
            option.quotes.ask_iv = JsonProcessor::parseString(quotesJson, "ask_iv");
            option.quotes.ask_size = JsonProcessor::parseString(quotesJson, "ask_size");
            option.quotes.best_ask = JsonProcessor::parseString(quotesJson, "best_ask");
            option.quotes.best_bid = JsonProcessor::parseString(quotesJson, "best_bid");
            option.quotes.bid_iv = JsonProcessor::parseString(quotesJson, "bid_iv");
            option.quotes.bid_size = JsonProcessor::parseString(quotesJson, "bid_size");
            option.quotes.impact_mid_price = JsonProcessor::parseString(quotesJson, "impact_mid_price");
            option.quotes.mark_iv = JsonProcessor::parseString(quotesJson, "mark_iv");

            option.size = JsonProcessor::parseNumber(value, "size");
            option.spot_price = JsonProcessor::parseString(value, "spot_price");
            option.strike_price = JsonProcessor::parseString(value, "strike_price");
            option.symbol = JsonProcessor::parseString(value, "symbol");
            option.timestamp = JsonProcessor::parseInteger(value, "timestamp");
            option.turnover = JsonProcessor::parseNumber(value, "turnover");
            option.turnover_symbol = JsonProcessor::parseString(value, "turnover_symbol");
            option.turnover_usd = JsonProcessor::parseNumber(value, "turnover_usd");
            option.volume = JsonProcessor::parseNumber(value, "volume");

            optionsVec.push_back(option);
        }
    }

    json_decref(root);
}
