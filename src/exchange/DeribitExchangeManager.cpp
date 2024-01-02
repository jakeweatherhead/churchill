#include "exchange/DeribitExchangeManager.h"

#include <iostream>

DeribitExchangeManager::DeribitExchangeManager() {}

DeribitExchangeManager::~DeribitExchangeManager() {}

std::string DeribitExchangeManager::fetchBtcFutures()
{
    static const std::string url = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=BTC&kind=future";
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

std::string DeribitExchangeManager::fetchBtcOptions()
{
    static const std::string url = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=BTC&kind=option";
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

std::string DeribitExchangeManager::fetchEthFutures()
{
    static const std::string url = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=ETH&kind=future";
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

std::string DeribitExchangeManager::fetchEthOptions()
{
    static const std::string url = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=ETH&kind=option";
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

std::vector<DeribitFutures> DeribitExchangeManager::parseFuturesToVector(const std::string &jsonStr)
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
    json_t *value, *field;

    std::vector<DeribitFutures> futuresVec;

    json_array_foreach(result, index, value)
    {
        DeribitFutures contract;

        try
        {
            field = json_object_get(value, "mid_price");
            contract.mid_price = JsonProcessor::parseNumber(field, "mid_price");

            field = json_object_get(value, "estimated_delivery_price");
            contract.estimated_delivery_price = JsonProcessor::parseNumber(field, "estimated_delivery_price");

            field = json_object_get(value, "volume_notional");
            contract.volume_notional = JsonProcessor::parseNumber(field, "volume_notional");

            field = json_object_get(value, "volume_usd");
            contract.volume_usd = JsonProcessor::parseNumber(field, "volume_usd");

            field = json_object_get(value, "quote_currency");
            contract.quote_currency = JsonProcessor::parseString(field);

            field = json_object_get(value, "creation_timestamp");
            contract.creation_timestamp = JsonProcessor::parseNumber(field, "creation_timestamp");

            field = json_object_get(value, "base_currency");
            contract.base_currency = JsonProcessor::parseString(field);

            field = json_object_get(value, "volume");
            contract.volume = JsonProcessor::parseNumber(field, "volume");

            field = json_object_get(value, "open_interest");
            contract.open_interest = JsonProcessor::parseNumber(field, "open_interest");

            field = json_object_get(value, "ask_price");
            contract.ask_price = JsonProcessor::parseNumber(field, "ask_price");

            field = json_object_get(value, "bid_price");
            contract.bid_price = JsonProcessor::parseNumber(field, "bid_price");

            field = json_object_get(value, "price_change");
            contract.price_change = JsonProcessor::parseNumber(field, "price_change");

            field = json_object_get(value, "mark_price");
            contract.mark_price = JsonProcessor::parseNumber(field, "mark_price");

            field = json_object_get(value, "instrument_name");
            contract.instrument_name = JsonProcessor::parseString(field);

            field = json_object_get(value, "last");
            contract.last = JsonProcessor::parseNumber(field, "last");

            field = json_object_get(value, "low");
            contract.low = JsonProcessor::parseNumber(field, "low");

            field = json_object_get(value, "high");
            contract.high = JsonProcessor::parseNumber(field, "high");
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << "DeribitExchangeManager, parseFuturesToVector error: " << e.what();
        }

        futuresVec.push_back(contract);
    }
    json_decref(root);
    return futuresVec;
}

std::vector<DeribitOption> DeribitExchangeManager::parseOptionsToVector(const std::string &jsonStr)
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
    json_t *value, *field;

    std::vector<DeribitOption> optionsVec;

    json_array_foreach(result, index, value)
    {
        DeribitOption option;

        try
        {
            field = json_object_get(value, "volume_usd");
            option.volume_usd = JsonProcessor::parseNumber(field, "volume_usd");

            field = json_object_get(value, "volume");
            option.volume = JsonProcessor::parseNumber(field, "volume");

            field = json_object_get(value, "underlying_price");
            option.underlying_price = JsonProcessor::parseNumber(field, "underlying_price");

            field = json_object_get(value, "underlying_index");
            option.underlying_index = JsonProcessor::parseString(field);

            field = json_object_get(value, "quote_currency");
            option.quote_currency = JsonProcessor::parseString(field);

            field = json_object_get(value, "price_change");
            option.price_change = JsonProcessor::parseNumber(field, "price_change");

            field = json_object_get(value, "open_interest");
            option.open_interest = JsonProcessor::parseNumber(field, "open_interest");

            field = json_object_get(value, "mid_price");
            option.mid_price = JsonProcessor::parseNumber(field, "mid_price");

            field = json_object_get(value, "mark_price");
            option.mark_price = JsonProcessor::parseNumber(field, "mark_price");

            field = json_object_get(value, "low");
            option.low = JsonProcessor::parseNumber(field, "low");

            field = json_object_get(value, "last");
            option.last = JsonProcessor::parseNumber(field, "last");

            field = json_object_get(value, "interest_rate");
            option.interest_rate = JsonProcessor::parseNumber(field, "interest_rate");

            field = json_object_get(value, "instrument_name");
            option.instrument_name = JsonProcessor::parseString(field);

            field = json_object_get(value, "high");
            option.high = JsonProcessor::parseNumber(field, "high");

            field = json_object_get(value, "estimated_delivery_price");
            option.estimated_delivery_price = JsonProcessor::parseNumber(field, "estimated_delivery_price");

            field = json_object_get(value, "creation_timestamp");
            option.creation_timestamp = JsonProcessor::parseInteger(field);

            field = json_object_get(value, "bid_price");
            option.bid_price = JsonProcessor::parseNumber(field, "bid_price");

            field = json_object_get(value, "base_currency");
            option.base_currency = JsonProcessor::parseString(field);

            field = json_object_get(value, "ask_price");
            option.ask_price = JsonProcessor::parseNumber(field, "ask_price");
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << "DeribitExchangeManager, parseOptionsToVector error: " << e.what();
        }
        optionsVec.push_back(option);
    }

    json_decref(root);
    return optionsVec;
}

DeribitOption DeribitExchangeManager::findDeribitComplement(const std::vector<DeribitOption> &optionsVec, const std::string &optionInstrumentName)
{
    for (int i = 0; i < optionsVec.size(); i++)
    {
        if (optionsVec[i].instrument_name == optionInstrumentName)
        {
            return optionsVec[i];
        }
    }
    throw std::runtime_error("Could not find deribit complement: " + optionInstrumentName + "\n");
}