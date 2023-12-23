#include "DeribitExchange/DeribitExchangeManager.h"

DeribitExchangeManager::DeribitExchangeManager()
{
    this->client = new RestClient();
}

DeribitExchangeManager::~DeribitExchangeManager()
{
    delete this->client;
}

std::string DeribitExchangeManager::HTTP_GET_BTC_Futures(bool writeToFile)
{
    static const std::string url = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=BTC&kind=future";
    std::string res = this->client->HTTP_GET(url);
    std::string formattedRes = JsonProcessor::formatJSON(res);
    if (writeToFile)
    {
        std::ofstream file;
        file.open("data/deribit_btc_futures.json");
        file << formattedRes;
        file.close();
    }
    return formattedRes;
}

std::string DeribitExchangeManager::HTTP_GET_BTC_Options(bool writeToFile)
{
    static const std::string url = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=BTC&kind=option";
    std::string res = this->client->HTTP_GET(url);
    std::string formattedRes = JsonProcessor::formatJSON(res);
    if (writeToFile)
    {
        std::ofstream file;
        file.open("data/deribit_btc_options.json");
        file << formattedRes;
        file.close();
    }
    return formattedRes;
}

std::string DeribitExchangeManager::HTTP_GET_ETH_Futures(bool writeToFile)
{
    static const std::string url = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=ETH&kind=future";
    std::string res = this->client->HTTP_GET(url);
    std::string formattedRes = JsonProcessor::formatJSON(res);
    if (writeToFile)
    {
        std::ofstream file;
        file.open("data/deribit_eth_futures.json");
        file << formattedRes;
        file.close();
    }
    return formattedRes;
}

std::string DeribitExchangeManager::HTTP_GET_ETH_Options(bool writeToFile)
{
    static const std::string url = "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=ETH&kind=option";
    std::string res = this->client->HTTP_GET(url);
    std::string formattedRes = JsonProcessor::formatJSON(res);
    if (writeToFile)
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

        contract.mid_price = JsonProcessor::getNumber(value, "mid_price");
        contract.estimated_delivery_price = JsonProcessor::getNumber(value, "estimated_delivery_price");
        contract.volume_notional = JsonProcessor::getNumber(value, "volume_notional");
        contract.volume_usd = JsonProcessor::getNumber(value, "volume_usd");
        contract.quote_currency = JsonProcessor::getNumber(value, "quote_currency");
        contract.creation_timestamp = JsonProcessor::getNumber(value, "creation_timestamp");
        contract.base_currency = JsonProcessor::getString(value, "base_currency");
        contract.volume = JsonProcessor::getNumber(value, "volume");
        contract.open_interest = JsonProcessor::getNumber(value, "open_interest");
        contract.ask_price = JsonProcessor::getNumber(value, "ask_price");
        contract.bid_price = JsonProcessor::getNumber(value, "bid_price");
        contract.price_change = JsonProcessor::getNumber(value, "price_change");
        contract.mark_price = JsonProcessor::getNumber(value, "mark_price");
        contract.instrument_name = JsonProcessor::getString(value, "instrument_name");
        contract.last = JsonProcessor::getNumber(value, "last");
        contract.low = JsonProcessor::getNumber(value, "low");
        contract.high = JsonProcessor::getNumber(value, "high");

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

        option.volume_usd = JsonProcessor::getNumber(value, "volume_usd");
        option.volume = JsonProcessor::getNumber(value, "volume");
        option.underlying_price = JsonProcessor::getNumber(value, "underlying_price");
        option.underlying_index = JsonProcessor::getString(value, "underlying_index");
        option.quote_currency = JsonProcessor::getString(value, "quote_currency");
        option.price_change = JsonProcessor::getNumber(value, "price_change");
        option.open_interest = JsonProcessor::getNumber(value, "open_interest");
        option.mid_price = JsonProcessor::getNumber(value, "mid_price");
        option.mark_price = JsonProcessor::getNumber(value, "mark_price");
        option.low = JsonProcessor::getNumber(value, "low");
        option.last = JsonProcessor::getNumber(value, "last");
        option.interest_rate = JsonProcessor::getNumber(value, "interest_rate");
        option.instrument_name = JsonProcessor::getString(value, "instrument_name");
        option.high = JsonProcessor::getNumber(value, "high");
        option.estimated_delivery_price = JsonProcessor::getNumber(value, "estimated_delivery_price");
        option.creation_timestamp = JsonProcessor::getInteger(value, "creation_timestamp");
        option.bid_price = JsonProcessor::getNumber(value, "bid_price");
        option.base_currency = JsonProcessor::getString(value, "base_currency");
        option.ask_price = JsonProcessor::getNumber(value, "ask_price");

        optionsVec.push_back(option);
    }

    json_decref(root); // Only need to decref the root; it will also decref child objects
}