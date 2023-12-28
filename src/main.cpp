#include "util/RestClient.h"
#include "util/JsonProcessor.h"
#include "util/OptionProcessor.h"
#include "util/Toolkit.h"

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
    std::chrono::duration<double> durations[NUM_RUNS] = {};

    DeribitExchangeManager *dbtEM = new DeribitExchangeManager();
    DeltaExchangeManager *dltEM = new DeltaExchangeManager();
    OptionProcessor *optionProcessor = new OptionProcessor();
    PCP_Strategy_0 *strategy = new PCP_Strategy_0();

    for (int i = 0; i < NUM_RUNS; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();

        std::string dbtBtcFuturesResponse;
        std::string dbtBtcOptionsResponse;
        std::string dbtEthFuturesResponse;
        std::string dbtEthOptionsResponse;
        std::string dltCallOptionsResponse;
        std::string dltPutOptionsResponse;

        // Fetch data from exchanges
        auto dbtBtcFuturesFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchBtcFutures, dbtEM);
        auto dbtBtcOptionsFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchBtcOptions, dbtEM);
        auto dbtEthFuturesFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchEthFutures, dbtEM);
        auto dbtEthOptionsFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchEthOptions, dbtEM);
        auto dltCallsFuture = std::async(std::launch::async, &DeltaExchangeManager::fetchOptions, dltEM, "call_options");
        auto dltPutsFuture = std::async(std::launch::async, &DeltaExchangeManager::fetchOptions, dltEM, "put_options");

        // Retrieve response 
        try
        {
            dbtBtcFuturesResponse = dbtBtcFuturesFuture.get();
            dbtBtcOptionsResponse = dbtBtcOptionsFuture.get();
            dbtEthFuturesResponse = dbtEthFuturesFuture.get();
            dbtEthOptionsResponse = dbtEthOptionsFuture.get();
            dltCallOptionsResponse = dltCallsFuture.get();
            dltPutOptionsResponse = dltPutsFuture.get();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what();
        }

        // Write response data
        Toolkit::writeToFile("data/dbt_btc_futures_res.json", dbtBtcFuturesResponse);
        Toolkit::writeToFile("data/dbt_btc_options_res.json", dbtBtcOptionsResponse);
        Toolkit::writeToFile("data/dbt_eth_futures_res.json", dbtEthFuturesResponse);
        Toolkit::writeToFile("data/dbt_eth_options_res.json", dbtEthOptionsResponse);
        Toolkit::writeToFile("data/dlt_call_options_res.json", dltCallOptionsResponse);
        Toolkit::writeToFile("data/dlt_put_options_res.json", dltPutOptionsResponse);

        //==--------------------------------------------------------------------==//

        std::vector<DeribitFutures> dbtBtcFuturesVec;
        std::vector<DeribitOption> dbtBtcOptionsVec;
        std::vector<DeltaOption> dltBtcCallOptionsVec;
        std::vector<DeltaOption> dltBtcPutOptionsVec;
        std::vector<DeribitFutures> dbtEthFuturesVec;
        std::vector<DeribitOption> dbtEthOptionsVec;
        std::vector<DeltaOption> dltEthCallOptionsVec;
        std::vector<DeltaOption> dltEthPutOptionsVec;

        try
        {
            dbtBtcFuturesVec = dbtEM->parseFuturesToVector(dbtBtcFuturesResponse);
            dbtBtcOptionsVec = dbtEM->parseOptionsToVector(dbtBtcOptionsResponse);
            dltBtcCallOptionsVec = dltEM->parseOptionsToVector("BTC", dltCallOptionsResponse);
            dltBtcPutOptionsVec = dltEM->parseOptionsToVector("BTC", dltPutOptionsResponse);
            dbtEthFuturesVec = dbtEM->parseFuturesToVector(dbtEthFuturesResponse);
            dbtEthOptionsVec = dbtEM->parseOptionsToVector(dbtEthOptionsResponse);
            dltEthCallOptionsVec = dltEM->parseOptionsToVector("ETH", dltCallOptionsResponse);
            dltEthPutOptionsVec = dltEM->parseOptionsToVector("ETH", dltPutOptionsResponse);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what();
        }

        //==--------------------------------------------------------------------==//

        std::vector<DeltaOption> dltBtcOptionsVec = dltBtcCallOptionsVec;
        dltBtcOptionsVec.insert(dltBtcOptionsVec.end(), dltBtcPutOptionsVec.begin(), dltBtcPutOptionsVec.end());
        std::vector<OptionPair> btcCandidates = optionProcessor->createOptionPairs("BTC", dbtBtcOptionsVec, dltBtcOptionsVec, dbtBtcFuturesVec);
        strategy->filterArbitrageOpportunities(btcCandidates);

        if (btcCandidates.size() == 0)
        {
            std::cout << "No BTC arbitrage opportunities found." << std::endl;
        }
        else
        {
            Toolkit::sortOptionPairsByReturnPerc(btcCandidates);
            std::string btcCandidatesStr = JsonProcessor::convertOptionPairsToString(btcCandidates);
            Toolkit::writeToFile("data/btc_ops.json", btcCandidatesStr);
        }

        //==--------------------------------------------------------------------==//

        std::vector<DeltaOption> dltEthOptionsVec = dltEthCallOptionsVec;
        dltEthOptionsVec.insert(dltEthOptionsVec.end(), dltEthPutOptionsVec.begin(), dltEthPutOptionsVec.end());
        std::vector<OptionPair> ethCandidates = optionProcessor->createOptionPairs("ETH", dbtEthOptionsVec, dltEthOptionsVec, dbtEthFuturesVec);
        strategy->filterArbitrageOpportunities(ethCandidates);

        if (ethCandidates.size() == 0)
        {
            std::cout << "No ETH arbitrage opportunities found." << std::endl;
        }
        else
        {
            Toolkit::sortOptionPairsByReturnPerc(ethCandidates);
            std::string ethCandidatesStr = JsonProcessor::convertOptionPairsToString(ethCandidates);
            Toolkit::writeToFile("data/eth_ops.json", ethCandidatesStr);
        }

        //==--------------------------------------==//

        auto finish = std::chrono::high_resolution_clock::now();
        durations[i] = finish - start;
    }

    //==--------------------------------------------------------------------==//

    // Get performance metrics
    double sum = 0;
    std::cout << "Number of runs: " << NUM_RUNS << std::endl;
    for (int i = 0; i < NUM_RUNS; i++)
    {
        std::cout << "Run-" << i
                  << " execution time: "
                  << durations[i].count()
                  << std::endl;
        sum += durations[i].count();
    }
    std::cout << "Average execution time: " << sum / NUM_RUNS << std::endl;

    delete dbtEM;
    delete dltEM;
    delete optionProcessor;
    delete strategy;
}
