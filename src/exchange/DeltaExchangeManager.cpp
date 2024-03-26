#include "exchange/DeltaExchangeManager.h"
#include "util/Toolkit.h"

#include <iostream>

DeltaExchangeManager::DeltaExchangeManager() {}

DeltaExchangeManager::~DeltaExchangeManager() {}

std::string DeltaExchangeManager::fetchOptions(std::string optionType)
{
    std::string url = "https://api.delta.exchange/v2/tickers?contract_types=" + optionType;

    std::string res;
    try
    {
        response = RestClient::HTTP_GET(url);
    }
    catch (const std::runtime_error &error)
    {
        std::cerr << error.what();
    }

    std::string formattedResponse = JsonProcessor::formatJSON(response);
    return formattedResponse;
}

std::vector<DeltaOption> DeltaExchangeManager::parseOptionsToVector(const std::string &currency, const std::string &jsonStr)
{
    json_error_t error;
    json_t *root = json_loads(jsonStr.c_str(), 0, &error);

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
    json_t *jsonObj;
    std::vector<DeltaOption> optionsVec;

    json_array_foreach(result, index, jsonObj)
    {
        std::string optionCurrency;
        try
        {
            optionCurrency = JsonProcessor::parseString(jsonObj, "oi_value_symbol");
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what();
        }

        if (optionCurrency == currency)
        {
            DeltaOption option;
            try
            {
                option.close = JsonProcessor::parseNumber(jsonObj, "close");
                option.contract_type = JsonProcessor::parseString(jsonObj, "contract_type");

                json_t *greeks = json_object_get(jsonObj, "greeks");
                option.greeks.delta = JsonProcessor::parseString(greeks, "delta");
                option.greeks.gamma = JsonProcessor::parseString(greeks, "gamma");
                option.greeks.rho = JsonProcessor::parseString(greeks, "rho");
                option.greeks.theta = JsonProcessor::parseString(greeks, "theta");
                option.greeks.vega = JsonProcessor::parseString(greeks, "vega");

                option.high = JsonProcessor::parseNumber(jsonObj, "high");
                option.low = JsonProcessor::parseNumber(jsonObj, "low");
                option.mark_price = JsonProcessor::parseString(jsonObj, "mark_price");
                option.mark_vol = JsonProcessor::parseString(jsonObj, "mark_vol");
                option.oi = JsonProcessor::parseString(jsonObj, "oi");
                option.oi_change_usd_6h = JsonProcessor::parseString(jsonObj, "oi_change_usd_6h");
                option.oi_contracts = JsonProcessor::parseString(jsonObj, "oi_contracts");
                option.oi_value = JsonProcessor::parseString(jsonObj, "oi_value");
                option.oi_value_symbol = JsonProcessor::parseString(jsonObj, "oi_value_symbol");
                option.oi_value_usd = JsonProcessor::parseString(jsonObj, "oi_value_usd");
                option.open = JsonProcessor::parseNumber(jsonObj, "open");

                json_t *priceBand = json_object_get(jsonObj, "price_band");
                option.price_band.lower_limit = JsonProcessor::parseString(priceBand, "lower_limit");
                option.price_band.upper_limit = JsonProcessor::parseString(priceBand, "upper_limit");

                option.product_id = JsonProcessor::parseInteger(jsonObj, "product_id");

                json_t *quotes = json_object_get(jsonObj, "quotes");
                option.quotes.ask_iv = JsonProcessor::parseString(quotes, "ask_iv");
                option.quotes.ask_size = JsonProcessor::parseString(quotes, "ask_size");
                option.quotes.best_ask = JsonProcessor::parseString(quotes, "best_ask");
                option.quotes.best_bid = JsonProcessor::parseString(quotes, "best_bid");
                option.quotes.bid_iv = JsonProcessor::parseString(quotes, "bid_iv");
                option.quotes.bid_size = JsonProcessor::parseString(quotes, "bid_size");
                option.quotes.impact_mid_price = JsonProcessor::parseString(quotes, "impact_mid_price");
                option.quotes.mark_iv = JsonProcessor::parseString(quotes, "mark_iv");

                option.size = JsonProcessor::parseNumber(jsonObj, "size");
                option.spot_price = JsonProcessor::parseString(jsonObj, "spot_price");
                option.strike_price = JsonProcessor::parseString(jsonObj, "strike_price");
                option.symbol = JsonProcessor::parseString(jsonObj, "symbol");
                option.timestamp = JsonProcessor::parseInteger(jsonObj, "timestamp");
                option.turnover = JsonProcessor::parseNumber(jsonObj, "turnover");
                option.turnover_symbol = JsonProcessor::parseString(jsonObj, "turnover_symbol");
                option.turnover_usd = JsonProcessor::parseNumber(jsonObj, "turnover_usd");
                option.volume = JsonProcessor::parseNumber(jsonObj, "volume");
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what();
            }

            optionsVec.push_back(option);
        }
    }

    json_decref(root);
    return optionsVec;
}

std::vector<std::string> DeltaExchangeManager::split(const std::string &input, const char &delimiter)
{

    if (input.empty())
    {
        throw std::invalid_argument("Input string is empty");
    }

    std::vector<std::string> parts;
    std::string part;
    std::istringstream tokenStream(input);

    while (std::getline(tokenStream, part, delimiter))
    {
        parts.push_back(part);
    }

    return parts;
}
