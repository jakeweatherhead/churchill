#include "exchange/DeribitExchangeManager.h"
#include "util/Toolkit.h"

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
    json_t *jsonObject;

    std::vector<DeribitFutures> futuresVec;

    json_array_foreach(result, index, jsonObject)
    {
        DeribitFutures contract;

        try
        {
            contract.mid_price = JsonProcessor::parseNumber(jsonObject, "mid_price");
            contract.estimated_delivery_price = JsonProcessor::parseNumber(jsonObject, "estimated_delivery_price");
            contract.volume_notional = JsonProcessor::parseNumber(jsonObject, "volume_notional");
            contract.volume_usd = JsonProcessor::parseNumber(jsonObject, "volume_usd");
            contract.quote_currency = JsonProcessor::parseString(jsonObject, "quote_currency");
            contract.creation_timestamp = JsonProcessor::parseNumber(jsonObject, "creation_timestamp");
            contract.base_currency = JsonProcessor::parseString(jsonObject, "base_currency");
            contract.volume = JsonProcessor::parseNumber(jsonObject, "volume");
            contract.open_interest = JsonProcessor::parseNumber(jsonObject, "open_interest");
            contract.ask_price = JsonProcessor::parseNumber(jsonObject, "ask_price");
            contract.bid_price = JsonProcessor::parseNumber(jsonObject, "bid_price");
            contract.price_change = JsonProcessor::parseNumber(jsonObject, "price_change");
            contract.mark_price = JsonProcessor::parseNumber(jsonObject, "mark_price");
            contract.instrument_name = JsonProcessor::parseString(jsonObject, "instrument_name");
            contract.last = JsonProcessor::parseNumber(jsonObject, "last");
            contract.low = JsonProcessor::parseNumber(jsonObject, "low");
            contract.high = JsonProcessor::parseNumber(jsonObject, "high");
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
    json_t *jsonObject;

    std::vector<DeribitOption> optionsVec;

    json_array_foreach(result, index, jsonObject)
    {
        DeribitOption option;

        try
        {
            option.volume_usd = JsonProcessor::parseNumber(jsonObject, "volume_usd");
            option.volume = JsonProcessor::parseNumber(jsonObject, "volume");
            option.underlying_price = JsonProcessor::parseNumber(jsonObject, "underlying_price");
            option.underlying_index = JsonProcessor::parseString(jsonObject, "underlying_index");
            option.quote_currency = JsonProcessor::parseString(jsonObject, "quote_currency");
            option.price_change = JsonProcessor::parseNumber(jsonObject, "price_change");
            option.open_interest = JsonProcessor::parseNumber(jsonObject, "open_interest");
            option.mid_price = JsonProcessor::parseNumber(jsonObject, "mid_price");
            option.mark_price = JsonProcessor::parseNumber(jsonObject, "mark_price");
            option.low = JsonProcessor::parseNumber(jsonObject, "low");
            option.last = JsonProcessor::parseNumber(jsonObject, "last");
            option.interest_rate = JsonProcessor::parseNumber(jsonObject, "interest_rate");
            option.instrument_name = JsonProcessor::parseString(jsonObject, "instrument_name");
            option.high = JsonProcessor::parseNumber(jsonObject, "high");
            option.estimated_delivery_price = JsonProcessor::parseNumber(jsonObject, "estimated_delivery_price");
            option.creation_timestamp = JsonProcessor::parseInteger(jsonObject, "creation_timestamp");
            option.bid_price = JsonProcessor::parseNumber(jsonObject, "bid_price");
            option.base_currency = JsonProcessor::parseString(jsonObject, "base_currency");
            option.ask_price = JsonProcessor::parseNumber(jsonObject, "ask_price");
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