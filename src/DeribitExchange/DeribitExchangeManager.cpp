#include "DeribitExchange/DeribitExchangeManager.h"
#include <iostream>

DeribitExchangeManager::DeribitExchangeManager()
{
}

DeribitExchangeManager::~DeribitExchangeManager()
{
}

std::string DeribitExchangeManager::fetchBtcFutures(bool writeData)
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
    if (writeData)
    {
        std::ofstream file;
        file.open("data/deribit_btc_futures.json");
        file << formattedRes;
        file.close();
    }
    return formattedRes;
}

std::string DeribitExchangeManager::fetchBtcOptions(bool writeData)
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
    if (writeData)
    {
        std::ofstream file;
        file.open("data/deribit_btc_options.json");
        file << formattedRes;
        file.close();
    }
    return formattedRes;
}

std::string DeribitExchangeManager::fetchEthFutures(bool writeData)
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
    if (writeData)
    {
        std::ofstream file;
        file.open("data/deribit_eth_futures.json");
        file << formattedRes;
        file.close();
    }
    return formattedRes;
}

std::string DeribitExchangeManager::fetchEthOptions(bool writeData)
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
    if (writeData)
    {
        std::ofstream file;
        file.open("data/deribit_eth_options.json");
        file << formattedRes;
        file.close();
    }
    return formattedRes;
}

void DeribitExchangeManager::parseFuturesToVector(const std::string &jsonString, std::vector<DeribitFutures> &futuresVec)
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
        DeribitFutures contract;

        contract.mid_price = JsonProcessor::parseNumber(value, "mid_price");
        contract.estimated_delivery_price = JsonProcessor::parseNumber(value, "estimated_delivery_price");
        contract.volume_notional = JsonProcessor::parseNumber(value, "volume_notional");
        contract.volume_usd = JsonProcessor::parseNumber(value, "volume_usd");
        contract.quote_currency = JsonProcessor::parseNumber(value, "quote_currency");
        contract.creation_timestamp = JsonProcessor::parseNumber(value, "creation_timestamp");
        contract.base_currency = JsonProcessor::parseString(value, "base_currency");
        contract.volume = JsonProcessor::parseNumber(value, "volume");
        contract.open_interest = JsonProcessor::parseNumber(value, "open_interest");
        contract.ask_price = JsonProcessor::parseNumber(value, "ask_price");
        contract.bid_price = JsonProcessor::parseNumber(value, "bid_price");
        contract.price_change = JsonProcessor::parseNumber(value, "price_change");
        contract.mark_price = JsonProcessor::parseNumber(value, "mark_price");
        contract.instrument_name = JsonProcessor::parseString(value, "instrument_name");
        contract.last = JsonProcessor::parseNumber(value, "last");
        contract.low = JsonProcessor::parseNumber(value, "low");
        contract.high = JsonProcessor::parseNumber(value, "high");

        futuresVec.push_back(contract);
    }

    json_decref(root); // Only need to decref the root; it will also decref child objects
}

void DeribitExchangeManager::parseOptionsToVector(const std::string &jsonString, std::vector<DeribitOption> &optionsVec)
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
        DeribitOption option;

        option.volume_usd = JsonProcessor::parseNumber(value, "volume_usd");
        option.volume = JsonProcessor::parseNumber(value, "volume");
        option.underlying_price = JsonProcessor::parseNumber(value, "underlying_price");
        option.underlying_index = JsonProcessor::parseString(value, "underlying_index");
        option.quote_currency = JsonProcessor::parseString(value, "quote_currency");
        option.price_change = JsonProcessor::parseNumber(value, "price_change");
        option.open_interest = JsonProcessor::parseNumber(value, "open_interest");
        option.mid_price = JsonProcessor::parseNumber(value, "mid_price");
        option.mark_price = JsonProcessor::parseNumber(value, "mark_price");
        option.low = JsonProcessor::parseNumber(value, "low");
        option.last = JsonProcessor::parseNumber(value, "last");
        option.interest_rate = JsonProcessor::parseNumber(value, "interest_rate");
        option.instrument_name = JsonProcessor::parseString(value, "instrument_name");
        option.high = JsonProcessor::parseNumber(value, "high");
        option.estimated_delivery_price = JsonProcessor::parseNumber(value, "estimated_delivery_price");
        option.creation_timestamp = JsonProcessor::parseInteger(value, "creation_timestamp");
        option.bid_price = JsonProcessor::parseNumber(value, "bid_price");
        option.base_currency = JsonProcessor::parseString(value, "base_currency");
        option.ask_price = JsonProcessor::parseNumber(value, "ask_price");

        optionsVec.push_back(option);
    }

    json_decref(root); // Only need to decref the root; it will also decref child objects
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
    throw std::runtime_error("Could not find deribit complement: " + optionInstrumentName);
}