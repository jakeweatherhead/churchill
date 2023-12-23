#include "DeltaExchange/DeltaExchangeManager.h"

#include <iostream>

DeltaExchangeManager::DeltaExchangeManager()
{
    client = new RestClient();
}

DeltaExchangeManager::~DeltaExchangeManager()
{
    delete client;
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
        std::string optionCurrency = JsonProcessor::getString(oi_value_symbolJson, "oi_value_symbol");

        if (optionCurrency == currency)
        {
            DeltaOption option;

            option.close = JsonProcessor::getNumber(value, "close");
            option.contract_type = JsonProcessor::getString(value, "contract_type");

            json_t *greeksJson = json_object_get(value, "greeks");
            option.greeks.delta = JsonProcessor::getString(greeksJson, "delta");
            option.greeks.gamma = JsonProcessor::getString(greeksJson, "gamma");
            option.greeks.rho = JsonProcessor::getString(greeksJson, "rho");
            option.greeks.theta = JsonProcessor::getString(greeksJson, "theta");
            option.greeks.vega = JsonProcessor::getString(greeksJson, "vega");

            option.high = JsonProcessor::getNumber(value, "high");
            option.low = JsonProcessor::getNumber(value, "low");
            option.mark_price = JsonProcessor::getString(value, "mark_price");
            option.mark_vol = JsonProcessor::getString(value, "mark_vol");
            option.oi = JsonProcessor::getString(value, "oi");
            option.oi_change_usd_6h = JsonProcessor::getString(value, "oi_change_usd_6h");
            option.oi_contracts = JsonProcessor::getString(value, "oi_contracts");
            option.oi_value = JsonProcessor::getString(value, "oi_value");
            option.oi_value_symbol = JsonProcessor::getString(value, "oi_value_symbol");
            option.oi_value_usd = JsonProcessor::getString(value, "oi_value_usd");
            option.open = JsonProcessor::getNumber(value, "open");

            json_t *priceBandJson = json_object_get(value, "price_band");
            option.price_band.lower_limit = JsonProcessor::getString(priceBandJson, "lower_limit");
            option.price_band.upper_limit = JsonProcessor::getString(priceBandJson, "upper_limit");

            json_t *quotesJson = json_object_get(value, "quotes");
            option.quotes.ask_iv = JsonProcessor::getString(quotesJson, "ask_iv");
            option.quotes.ask_size = JsonProcessor::getString(quotesJson, "ask_size");
            option.quotes.best_ask = JsonProcessor::getString(quotesJson, "best_ask");
            option.quotes.best_bid = JsonProcessor::getString(quotesJson, "best_bid");
            option.quotes.bid_iv = JsonProcessor::getString(quotesJson, "bid_iv");
            option.quotes.bid_size = JsonProcessor::getString(quotesJson, "bid_size");
            option.quotes.impact_mid_price = JsonProcessor::getString(quotesJson, "impact_mid_price");
            option.quotes.mark_iv = JsonProcessor::getString(quotesJson, "mark_iv");

            option.size = JsonProcessor::getNumber(value, "size");
            option.spot_price = JsonProcessor::getString(value, "spot_price");
            option.strike_price = JsonProcessor::getString(value, "strike_price");
            option.symbol = JsonProcessor::getString(value, "symbol");
            option.timestamp = JsonProcessor::getInteger(value, "timestamp");
            option.turnover = JsonProcessor::getNumber(value, "turnover");
            option.turnover_symbol = JsonProcessor::getString(value, "turnover_symbol");
            option.turnover_usd = JsonProcessor::getNumber(value, "turnover_usd");
            option.volume = JsonProcessor::getNumber(value, "volume");

            optionsVec.push_back(option);
        }
    }

    json_decref(root);
}
