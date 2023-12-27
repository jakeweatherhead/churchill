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

    DeribitExchangeManager *dbtEM = new DeribitExchangeManager();
    DeltaExchangeManager *dltEM = new DeltaExchangeManager();
    OptionProcessor *optionProcessor = new OptionProcessor();
    PCP_Strategy_0 *strategy = new PCP_Strategy_0();

    std::chrono::duration<double> durations[NUM_RUNS] = {};
    for (int i = 0; i < NUM_RUNS; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();

        auto dbtBtcFuturesFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchBtcFutures, dbtEM, WRITE_DATA);
        auto dbtBTCOptionsFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchBtcOptions, dbtEM, WRITE_DATA);
        auto dbtETHFuturesFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchEthFutures, dbtEM, WRITE_DATA);
        auto dbtETHOptionsFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchEthOptions, dbtEM, WRITE_DATA);

        auto dltCallOptionsFuture = std::async(std::launch::async, &DeltaExchangeManager::fetchOptions, dltEM, "call_options", WRITE_DATA);
        auto dltPutOptionsFuture = std::async(std::launch::async, &DeltaExchangeManager::fetchOptions, dltEM, "put_options", WRITE_DATA);

        std::string dbtBtcFutures;
        std::string dbtBtcOptions;
        std::string dbtEthFutures;
        std::string dbtEthOptions;

        std::string dltCallOptions;
        std::string dltPutOptions;

        try
        {
            dbtBtcFutures = dbtBtcFuturesFuture.get();
            dbtBtcOptions = dbtBTCOptionsFuture.get();
            dbtEthFutures = dbtETHFuturesFuture.get();
            dbtEthOptions = dbtETHOptionsFuture.get();

            dltCallOptions = dltCallOptionsFuture.get();
            dltPutOptions = dltPutOptionsFuture.get();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what();
        }

        //==--------------------------------------------------------------------==//

        std::vector<DeribitFutures> dbtBtcFuturesVec;
        std::vector<DeribitOption> dbtBtcOptionsVec;

        std::vector<DeribitFutures> dbtEthFuturesVec;
        std::vector<DeribitOption> dbtEthOptionsVec;

        std::vector<DeltaOption> dltBtcCallOptionsVec;
        std::vector<DeltaOption> dltBtcPutOptionsVec;

        std::vector<DeltaOption> dltEthCallOptionsVec;
        std::vector<DeltaOption> dltEthPutOptionsVec;

        try
        {
            dbtEM->parseFuturesToVector(dbtBtcFutures, dbtBtcFuturesVec);
            dbtEM->parseOptionsToVector(dbtBtcOptions, dbtBtcOptionsVec);

            dbtEM->parseFuturesToVector(dbtEthFutures, dbtEthFuturesVec);
            dbtEM->parseOptionsToVector(dbtEthOptions, dbtEthOptionsVec);

            dltEM->parseOptionsToVector("BTC", dltCallOptions, dltBtcCallOptionsVec);
            dltEM->parseOptionsToVector("BTC", dltPutOptions, dltBtcPutOptionsVec);

            dltEM->parseOptionsToVector("ETH", dltCallOptions, dltEthCallOptionsVec);
            dltEM->parseOptionsToVector("ETH", dltPutOptions, dltEthPutOptionsVec);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what();
        }

        // Combine delta options (BTC)
        std::vector<DeltaOption> dltBtcOptionsVec = dltBtcCallOptionsVec;
        dltBtcOptionsVec.insert(dltBtcOptionsVec.end(), dltBtcPutOptionsVec.begin(), dltBtcPutOptionsVec.end());

        // Combine delta options (ETH)
        std::vector<DeltaOption> dltEthOptionsVec = dltEthCallOptionsVec;
        dltEthOptionsVec.insert(dltEthOptionsVec.end(), dltEthPutOptionsVec.begin(), dltEthPutOptionsVec.end());

        // Process BTC candidates
        std::vector<OptionPair> btcCandidates = optionProcessor->createOptionPairs("BTC", dbtBtcOptionsVec, dltBtcOptionsVec, dbtBtcFuturesVec);
        strategy->filterArbitrageOpportunities(btcCandidates);

        if (btcCandidates.size() == 0)
        {
            std::cout << "No BTC arbitrage opportunities found." << std::endl;
        }
        else
        {
            std::string btcCandidatesStr = JsonProcessor::convertOptionPairsToString(btcCandidates);
            std::string formattedRes = JsonProcessor::formatJSON(btcCandidatesStr);
            std::ofstream file;
            file.open("data/candidates.json");
            file << formattedRes;
            file.close();
        }

        // Process ETH candidates
        std::vector<OptionPair> ethCandidates = optionProcessor->createOptionPairs("ETH", dbtEthOptionsVec, dltEthOptionsVec, dbtEthFuturesVec);
        strategy->filterArbitrageOpportunities(ethCandidates);

        if (ethCandidates.size() == 0)
        {
            std::cout << "No ETH arbitrage opportunities found." << std::endl;
        }
        else
        {
            std::string ethCandidatesStr = JsonProcessor::convertOptionPairsToString(ethCandidates);
            std::string formattedRes = JsonProcessor::formatJSON(ethCandidatesStr);
            std::ofstream file;
            file.open("data/candidates.json");
            file << formattedRes;
            file.close();
        }

        auto finish = std::chrono::high_resolution_clock::now();
        durations[i] = finish - start;
    }

    // Get performance metrics
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

    delete dbtEM;
    delete dltEM;
}
