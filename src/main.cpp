#include "util/RestClient.h"
#include "util/JsonProcessor.h"
#include "util/OptionProcessor.h"

#include "DeribitExchange/DeribitExchangeManager.h"

#include "DeltaExchange/DeltaExchangeManager.h"
#include "DeltaExchange/DeltaOption.h"

#include "PutCallParity/PCP_Strategy_0.h"

#include <iostream>
#include <chrono>
#include <future>

int main()
{
    int NUM_RUNS = 1;
    bool WRITE_DATA = true;

    DeribitExchangeManager *deribitEM = new DeribitExchangeManager();
    DeltaExchangeManager *deltaEM = new DeltaExchangeManager();
    OptionProcessor *optionProcessor = new OptionProcessor();
    PCP_Strategy_0 *strategy = new PCP_Strategy_0();

    std::chrono::duration<double> durations[NUM_RUNS] = {};
    for (int i = 0; i < NUM_RUNS; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();

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

        std::vector<OptionPair> candidates;
        optionProcessor->createOptionPairs("BTC", deribitBTCOptionsVec, deltaBTCCallOptionsVec, deltaPutOptionsVec, candidates);
        strategy->filterArbitrageOpportunities(candidates);

        if (candidates.size() == 0)
        {
            std::cout << "No arbitrage opportunities found." << std::endl;
        }
        else
        {
            std::string candidatesStr = JsonProcessor::convertOptionPairsToString(candidates);
            std::string formattedRes = JsonProcessor::formatJSON(candidatesStr);
            std::ofstream file;
            file.open("data/candidates.json");
            file << formattedRes;
            file.close();
        }

        auto finish = std::chrono::high_resolution_clock::now();
        durations[i] = finish - start;
    }

    double sum = 0;
    std::cout << "Number of runs: " << NUM_RUNS << std::endl;
    for (int i = 0; i < NUM_RUNS; i++)
    {
        std::cout << "(" << i << ")"
                  << " execution time: "
                  << durations[i].count() << std::endl;
        sum += durations[i].count();
    }
    std::cout << "Average execution time: " << sum / NUM_RUNS << std::endl;

    delete deribitEM;
    delete deltaEM;
}
