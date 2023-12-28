#include "DeltaExchange/DeltaExchangeManager.h"

#include <iostream>

DeltaExchangeManager::DeltaExchangeManager() {}

DeltaExchangeManager::~DeltaExchangeManager() {}

std::string DeltaExchangeManager::fetchOptions(std::string optionType)
{
    std::string url = "https://api.delta.exchange/v2/tickers?contract_types=" + optionType;

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
    return formattedRes;
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
    json_t *value, *greeks, *priceBand, *quotes, *field;

    std::vector<DeltaOption> optionsVec;

    json_array_foreach(result, index, value)
    {
        json_t *oi_value_symbolJson = json_object_get(value, "oi_value_symbol");
        std::string optionCurrency = JsonProcessor::parseString(oi_value_symbolJson);

        if (optionCurrency == currency)
        {
            DeltaOption option;

            field = json_object_get(value, "close");
            option.close = JsonProcessor::parseNumber(field, "close");

            field = json_object_get(value, "contract_type");
            option.contract_type = JsonProcessor::parseString(field);

            //==-------------------------------------------------------==//
            // Greeks

            greeks = json_object_get(value, "greeks");

            field = json_object_get(greeks, "delta");
            option.greeks.delta = JsonProcessor::parseString(field);

            field = json_object_get(greeks, "gamma");
            option.greeks.gamma = JsonProcessor::parseString(field);

            field = json_object_get(greeks, "rho");
            option.greeks.rho = JsonProcessor::parseString(field);

            field = json_object_get(greeks, "theta");
            option.greeks.theta = JsonProcessor::parseString(field);

            field = json_object_get(greeks, "vega");
            option.greeks.vega = JsonProcessor::parseString(field);

            //==-------------------------------------------------------==//

            field = json_object_get(value, "high");
            option.high = JsonProcessor::parseNumber(field, "high");

            field = json_object_get(value, "low");
            option.low = JsonProcessor::parseNumber(field, "low");

            field = json_object_get(value, "mark_price");
            option.mark_price = JsonProcessor::parseString(field);

            field = json_object_get(value, "mark_vol");
            option.mark_vol = JsonProcessor::parseString(field);

            field = json_object_get(value, "oi");
            option.oi = JsonProcessor::parseString(field);

            field = json_object_get(value, "oi_change_usd_6h");
            option.oi_change_usd_6h = JsonProcessor::parseString(field);

            field = json_object_get(value, "oi_contracts");
            option.oi_contracts = JsonProcessor::parseString(field);

            field = json_object_get(value, "oi_value");
            option.oi_value = JsonProcessor::parseString(field);

            field = json_object_get(value, "oi_value_symbol");
            option.oi_value_symbol = JsonProcessor::parseString(field);

            field = json_object_get(value, "oi_value_usd");
            option.oi_value_usd = JsonProcessor::parseString(field);

            field = json_object_get(value, "open");
            option.open = JsonProcessor::parseNumber(field, "open");

            //==-------------------------------------------------------==//
            // Price band

            priceBand = json_object_get(value, "price_band");

            field = json_object_get(priceBand, "lower_limit");
            option.price_band.lower_limit = JsonProcessor::parseString(field);

            field = json_object_get(priceBand, "upper_limit");
            option.price_band.upper_limit = JsonProcessor::parseString(field);

            //==-------------------------------------------------------==//

            field = json_object_get(value, "product_id");
            option.product_id = JsonProcessor::parseInteger(field);

            //==-------------------------------------------------------==//
            // Quotes

            quotes = json_object_get(value, "quotes");

            field = json_object_get(quotes, "ask_iv");
            option.quotes.ask_iv = JsonProcessor::parseString(field);

            field = json_object_get(quotes, "ask_size");
            option.quotes.ask_size = JsonProcessor::parseString(field);

            field = json_object_get(quotes, "best_ask");
            option.quotes.best_ask = JsonProcessor::parseString(field);

            field = json_object_get(quotes, "best_bid");
            option.quotes.best_bid = JsonProcessor::parseString(field);

            field = json_object_get(quotes, "bid_iv");
            option.quotes.bid_iv = JsonProcessor::parseString(field);

            field = json_object_get(quotes, "bid_size");
            option.quotes.bid_size = JsonProcessor::parseString(field);

            field = json_object_get(quotes, "impact_mid_price");
            option.quotes.impact_mid_price = JsonProcessor::parseString(field);

            field = json_object_get(quotes, "mark_iv");
            option.quotes.mark_iv = JsonProcessor::parseString(field);

            //==-------------------------------------------------------==//
            
            field = json_object_get(value, "size");
            option.size = JsonProcessor::parseNumber(field, "size");

            field = json_object_get(value, "spot_price");
            option.spot_price = JsonProcessor::parseString(field);

            field = json_object_get(value, "strike_price");
            option.strike_price = JsonProcessor::parseString(field);

            field = json_object_get(value, "symbol");
            option.symbol = JsonProcessor::parseString(field);

            field = json_object_get(value, "timestamp");
            option.timestamp = JsonProcessor::parseInteger(field);

            field = json_object_get(value, "turnover");
            option.turnover = JsonProcessor::parseNumber(field, "turnover");

            field = json_object_get(value, "turnover_symbol");
            option.turnover_symbol = JsonProcessor::parseString(field);

            field = json_object_get(value, "turnover_usd");
            option.turnover_usd = JsonProcessor::parseNumber(field, "turnover_usd");

            field = json_object_get(value, "volume");
            option.volume = JsonProcessor::parseNumber(field, "volume");

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
