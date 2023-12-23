#include "util/RestClient.h"
#include "util/JsonProcessor.h"
#include "util/OptionProcessor.h"
#include "DeribitExchange/DeribitExchangeManager.h"
#include "DeltaExchange/DeltaExchangeManager.h"
#include "DeltaExchange/DeltaOption.h"

#include <iostream>
#include <chrono>

int main()
{
    int NUM_RUNS = 1;
    bool WRITE_TO_FILE = true;

    DeribitExchangeManager *deribitEM = new DeribitExchangeManager();
    DeltaExchangeManager *deltaEM = new DeltaExchangeManager();
    OptionProcessor *optionProcessor = new OptionProcessor();

    std::chrono::duration<double> durations[NUM_RUNS] = {};
    for (int i = 0; i < NUM_RUNS; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();

        std::string deribitBTCFutures = deribitEM->getBitcoinFutures(WRITE_TO_FILE);
        std::string deribitBTCOptions = deribitEM->getBitcoinOptions(WRITE_TO_FILE);
        std::string deribitETHFutures = deribitEM->getEtherFutures(WRITE_TO_FILE);
        std::string deribitETHOptions = deribitEM->getEtherOptions(WRITE_TO_FILE);

        std::string deltaCallOptions = deltaEM->getOptions("call_options", WRITE_TO_FILE);
        std::string deltaPutOptions = deltaEM->getOptions("put_options", WRITE_TO_FILE);

        std::vector<DeribitFutures> deribitBTCFuturesVec;
        std::vector<DeribitOption> deribitBTCOptionsVec;
        std::vector<DeribitFutures> deribitETHFuturesVec;
        std::vector<DeribitOption> deribitETHOptionsVec;

        std::vector<DeltaOption> deltaCallOptionsVec;
        std::vector<DeltaOption> deltaPutOptionsVec;
        std::vector<DeltaOption> deltaBTCCallOptionsVec;
        std::vector<DeltaOption> deltaETHCallOptionsVec;
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
            std::cout << "[main.cpp] Error parsing: " << e.what() << std::endl;
        }

        auto finish = std::chrono::high_resolution_clock::now();
        durations[i] = finish - start;
    }

    double sum = 0;
    for (int i = 0; i < NUM_RUNS; i++)
    {
        std::cout << "Execution time: " << durations[i].count() << std::endl;
        sum += durations[i].count();
    }
    std::cout << "Average execution time: " << sum / NUM_RUNS << std::endl;

    delete deribitEM;
    delete deltaEM;
}
