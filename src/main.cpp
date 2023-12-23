#include "util/RestClient.h"
#include "util/JsonProcessor.h"
#include "util/OptionProcessor.h"
#include "DeribitExchange/DeribitExchangeManager.h"
#include "DeltaExchange/DeltaExchangeManager.h"
#include "DeltaExchange/DeltaOption.h"

#include <iostream>
#include <chrono>
#include <future>

int main()
{
    int NUM_RUNS = 1;
    bool WRITE_DATA = false;

    DeribitExchangeManager *deribitEM = new DeribitExchangeManager();
    DeltaExchangeManager *deltaEM = new DeltaExchangeManager();
    OptionProcessor *optionProcessor = new OptionProcessor();

    std::chrono::duration<double> durations[NUM_RUNS] = {};
    for (int i = 0; i < NUM_RUNS; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();

        // std::string deribitBTCFutures = deribitEM->getBitcoinFutures(WRITE_DATA);
        // std::string deribitBTCOptions = deribitEM->getBitcoinOptions(WRITE_DATA);
        // std::string deribitETHFutures = deribitEM->getEtherFutures(WRITE_DATA);
        // std::string deribitETHOptions = deribitEM->getEtherOptions(WRITE_DATA);

        // std::string deltaCallOptions = deltaEM->getOptions("call_options", WRITE_DATA);
        // std::string deltaPutOptions = deltaEM->getOptions("put_options", WRITE_DATA);

        std::vector<DeribitFutures> deribitBTCFuturesVec;
        std::vector<DeribitOption> deribitBTCOptionsVec;
        std::vector<DeribitFutures> deribitETHFuturesVec;
        std::vector<DeribitOption> deribitETHOptionsVec;

        std::vector<DeltaOption> deltaCallOptionsVec;
        std::vector<DeltaOption> deltaPutOptionsVec;
        std::vector<DeltaOption> deltaBTCCallOptionsVec;
        std::vector<DeltaOption> deltaETHCallOptionsVec;

        auto deribitBTCFuturesFuture = std::async(std::launch::async, &DeribitExchangeManager::getBitcoinFutures, deribitEM, WRITE_DATA);
        auto deribitBTCOptionsFuture = std::async(std::launch::async, &DeribitExchangeManager::getBitcoinOptions, deribitEM, WRITE_DATA);
        auto deribitETHFuturesFuture = std::async(std::launch::async, &DeribitExchangeManager::getEtherFutures, deribitEM, WRITE_DATA);
        auto deribitETHOptionsFuture = std::async(std::launch::async, &DeribitExchangeManager::getEtherOptions, deribitEM, WRITE_DATA);

        auto deltaCallOptionsFuture = std::async(std::launch::async, &DeltaExchangeManager::getOptions, deltaEM, "call_options", WRITE_DATA);
        auto deltaPutOptionsFuture = std::async(std::launch::async, &DeltaExchangeManager::getOptions, deltaEM, "put_options", WRITE_DATA);

        std::string deribitBTCFutures;
        std::string deribitBTCOptions;
        std::string deribitETHFutures;
        std::string deribitETHOptions;
        std::string deltaCallOptions;
        std::string deltaPutOptions;

        try
        {
            deribitBTCFutures = deribitBTCFuturesFuture.get();
            deribitBTCOptions = deribitBTCOptionsFuture.get();
            deribitETHFutures = deribitETHFuturesFuture.get();
            deribitETHOptions = deribitETHOptionsFuture.get();

            deltaCallOptions = deltaCallOptionsFuture.get();
            deltaPutOptions = deltaPutOptionsFuture.get();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what();
        }

        try
        {
            deribitEM->parseFuturesToVector(deribitBTCFutures, deribitBTCFuturesVec);
            deribitEM->parseOptionsToVector(deribitBTCOptions, deribitBTCOptionsVec);
            deribitEM->parseOptionsToVector(deribitETHOptions, deribitETHOptionsVec);
            deribitEM->parseFuturesToVector(deribitETHFutures, deribitETHFuturesVec);

            deltaEM->parseOptionsToVector("BTC", deltaCallOptions, deltaBTCCallOptionsVec);
            deltaEM->parseOptionsToVector("ETH", deltaCallOptions, deltaETHCallOptionsVec);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what();
        }

        auto finish = std::chrono::high_resolution_clock::now();
        durations[i] = finish - start;
    }

    double sum = 0;
    for (int i = 0; i < NUM_RUNS; i++)
    {
        std::cout << "Execution time: " << durations[i].count() << ", NUM_RUNS: " << NUM_RUNS << std::endl;
        sum += durations[i].count();
    }
    std::cout << "Average execution time: " << sum / NUM_RUNS << std::endl;

    delete deribitEM;
    delete deltaEM;
}
