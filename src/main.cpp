#include "util/RestClient.h"
#include "util/JsonProcessor.h"
#include "util/OptionProcessor.h"
#include "util/Toolkit.h"

#include "exchange/DeribitExchangeManager.h"
#include "exchange/DeltaExchangeManager.h"
#include "exchange/DeltaOption.h"

#include "strategy/PCP_Strategy_0.h"

#include <iostream>
#include <chrono>
#include <future>
#include <thread>

std::string BLACK = "\033[0;30m";
std::string RED = "\033[0;31m";
std::string GREEN = "\033[0;32m";
std::string YELLOW = "\033[0;33m";
std::string BLUE = "\033[0;34m";
std::string MAGENTA = "\033[0;35m";
std::string CYAN = "\033[0;36m";
std::string WHITE = "\033[0;37m";
std::string B_BLACK = "\033[1;30m";
std::string B_RED = "\033[1;31m";
std::string B_GREEN = "\033[1;32m";
std::string B_YELLOW = "\033[1;33m";
std::string B_BLUE = "\033[1;34m";
std::string B_MAGENTA = "\033[1;35m";
std::string B_CYAN = "\033[1;36m";
std::string B_WHITE = "\033[1;37m";
std::string RESET = "\033[0m\n";

const int NUM_RUNS = 1;
std::chrono::duration<double> durations[NUM_RUNS] = {};
DeribitExchangeManager *dbtEM = new DeribitExchangeManager();
DeltaExchangeManager *dltEM = new DeltaExchangeManager();
OptionProcessor *optionProcessor = new OptionProcessor();
PCP_Strategy_0 *pcpStrategy_0 = new PCP_Strategy_0();
std::vector<OptionPair> btcCandidates;
std::vector<OptionPair> ethCandidates;

struct ExchangeDataFutures
{
    std::future<std::string> dbtBtcFuturesFuture;
    std::future<std::string> dbtBtcOptionsFuture;
    std::future<std::string> dbtEthFuturesFuture;
    std::future<std::string> dbtEthOptionsFuture;
    std::future<std::string> dltCallsFuture;
    std::future<std::string> dltPutsFuture;
};

struct ExchangeDataResponses
{
    std::string dbtBtcFuturesResponse;
    std::string dbtBtcOptionsResponse;
    std::string dbtEthFuturesResponse;
    std::string dbtEthOptionsResponse;
    std::string dltCallOptionsResponse;
    std::string dltPutOptionsResponse;
};

struct ExchangeDataVectors
{
    std::vector<DeribitFutures> dbtBtcFuturesVec;
    std::vector<DeribitOption> dbtBtcOptionsVec;
    std::vector<DeribitFutures> dbtEthFuturesVec;
    std::vector<DeribitOption> dbtEthOptionsVec;
    std::vector<DeltaOption> dltBtcCallOptionsVec;
    std::vector<DeltaOption> dltBtcPutOptionsVec;
    std::vector<DeltaOption> dltEthCallOptionsVec;
    std::vector<DeltaOption> dltEthPutOptionsVec;
};

ExchangeDataFutures fetchData()
{
    ExchangeDataFutures futures;
    futures.dbtBtcFuturesFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchBtcFutures, dbtEM);
    futures.dbtBtcOptionsFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchBtcOptions, dbtEM);
    futures.dbtEthFuturesFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchEthFutures, dbtEM);
    futures.dbtEthOptionsFuture = std::async(std::launch::async, &DeribitExchangeManager::fetchEthOptions, dbtEM);
    futures.dltCallsFuture = std::async(std::launch::async, &DeltaExchangeManager::fetchOptions, dltEM, "call_options");
    futures.dltPutsFuture = std::async(std::launch::async, &DeltaExchangeManager::fetchOptions, dltEM, "put_options");
    return futures;
}

ExchangeDataResponses retrieveResponses(ExchangeDataFutures &futures)
{
    ExchangeDataResponses responses;
    try
    {
        responses.dbtBtcFuturesResponse = futures.dbtBtcFuturesFuture.get();
        responses.dbtBtcOptionsResponse = futures.dbtBtcOptionsFuture.get();
        responses.dbtEthFuturesResponse = futures.dbtEthFuturesFuture.get();
        responses.dbtEthOptionsResponse = futures.dbtEthOptionsFuture.get();
        responses.dltCallOptionsResponse = futures.dltCallsFuture.get();
        responses.dltPutOptionsResponse = futures.dltPutsFuture.get();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in retrieveResponses: " << e.what() << std::endl;
    }
    return responses;
}

void writeResponses(ExchangeDataResponses &responses)
{
    std::vector<std::thread> threads;

    // Spawn threads
    threads.emplace_back(Toolkit::writeJsonToFile, "data/pcp_0/res/dbt_btc_futures_res.json", responses.dbtBtcFuturesResponse);
    threads.emplace_back(Toolkit::writeJsonToFile, "data/pcp_0/res/dbt_btc_options_res.json", responses.dbtBtcOptionsResponse);
    threads.emplace_back(Toolkit::writeJsonToFile, "data/pcp_0/res/dbt_eth_futures_res.json", responses.dbtEthFuturesResponse);
    threads.emplace_back(Toolkit::writeJsonToFile, "data/pcp_0/res/dbt_eth_options_res.json", responses.dbtEthOptionsResponse);
    threads.emplace_back(Toolkit::writeJsonToFile, "data/pcp_0/res/dlt_call_options_res.json", responses.dltCallOptionsResponse);
    threads.emplace_back(Toolkit::writeJsonToFile, "data/pcp_0/res/dlt_put_options_res.json", responses.dltPutOptionsResponse);

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
}

ExchangeDataVectors parseResponsesToVectors(ExchangeDataResponses &responses)
{
    ExchangeDataVectors vectors;
    std::vector<std::thread> threads;

    try
    {
        threads.emplace_back([&]()
                             { vectors.dbtBtcFuturesVec = dbtEM->parseFuturesToVector(responses.dbtBtcFuturesResponse); });
        threads.emplace_back([&]()
                             { vectors.dbtBtcOptionsVec = dbtEM->parseOptionsToVector(responses.dbtBtcOptionsResponse); });
        threads.emplace_back([&]()
                             { vectors.dbtEthFuturesVec = dbtEM->parseFuturesToVector(responses.dbtEthFuturesResponse); });
        threads.emplace_back([&]()
                             { vectors.dbtEthOptionsVec = dbtEM->parseOptionsToVector(responses.dbtEthOptionsResponse); });

        threads.emplace_back([&]()
                             { vectors.dltBtcCallOptionsVec = dltEM->parseOptionsToVector("BTC", responses.dltCallOptionsResponse); });
        threads.emplace_back([&]()
                             { vectors.dltBtcPutOptionsVec = dltEM->parseOptionsToVector("BTC", responses.dltPutOptionsResponse); });
        threads.emplace_back([&]()
                             { vectors.dltEthCallOptionsVec = dltEM->parseOptionsToVector("ETH", responses.dltCallOptionsResponse); });
        threads.emplace_back([&]()
                             { vectors.dltEthPutOptionsVec = dltEM->parseOptionsToVector("ETH", responses.dltPutOptionsResponse); });

        // Wait for all threads to finish
        for (auto &t : threads)
        {
            if (t.joinable())
            {
                t.join();
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what();
    }

    return vectors;
}

void analyseBtcData(ExchangeDataVectors &vectors)
{
    std::vector<DeltaOption> dltOptionsVec = vectors.dltBtcCallOptionsVec;
    dltOptionsVec.insert(dltOptionsVec.end(), vectors.dltBtcPutOptionsVec.begin(), vectors.dltBtcPutOptionsVec.end());
    btcCandidates = optionProcessor->createOptionPairs("BTC", vectors.dbtBtcOptionsVec, dltOptionsVec, vectors.dbtBtcFuturesVec);
    btcCandidates = pcpStrategy_0->filterArbitrageOpportunities(btcCandidates);

    if (btcCandidates.size() == 0)
    {
        std::cout << "No BTC arbitrage opportunities found." << std::endl;
    }
    else
    {
        Toolkit::sortOptionPairsByReturnPerc(btcCandidates);
        std::string candidates = JsonProcessor::convertOptionPairsToString(btcCandidates);
        std::string filename = "data/pcp_0/arb/BTC_ops.json";
        Toolkit::writeJsonToFile(filename, candidates);
    }
}

void analyseEthData(ExchangeDataVectors &vectors)
{
    std::vector<DeltaOption> dltOptionsVec = vectors.dltEthCallOptionsVec;
    dltOptionsVec.insert(dltOptionsVec.end(), vectors.dltEthPutOptionsVec.begin(), vectors.dltEthPutOptionsVec.end());
    ethCandidates = optionProcessor->createOptionPairs("ETH", vectors.dbtEthOptionsVec, dltOptionsVec, vectors.dbtEthFuturesVec);
    ethCandidates = pcpStrategy_0->filterArbitrageOpportunities(ethCandidates);

    if (ethCandidates.size() == 0)
    {
        std::cout << "No ETH arbitrage opportunities found." << std::endl;
    }
    else
    {
        Toolkit::sortOptionPairsByReturnPerc(ethCandidates);
        std::string candidates = JsonProcessor::convertOptionPairsToString(ethCandidates);
        std::string filename = "data/pcp_0/arb/ETH_ops.json";
        Toolkit::writeJsonToFile(filename, candidates);
    }
}

void reportResults()
{
    std::cout << "-----------------------------------\n";
    std::cout << "BTC Arbitrage Opportunities: " << btcCandidates.size() << std::endl;
    std::cout << "ETH Arbitrage Opportunities: " << ethCandidates.size() << std::endl;
}

void freeMemory()
{
    delete dbtEM;
    delete dltEM;
    delete optionProcessor;
    delete pcpStrategy_0;
}

int main()
{
    for (int i = 0; i < NUM_RUNS; i++)
    {
        // auto start = std::chrono::high_resolution_clock::now(); // profiling

        // std::chrono::duration<double> subdurations[6] = {}; // profiling

        // auto s = std::chrono::high_resolution_clock::now(); // profiling
        ExchangeDataFutures futures = fetchData();
        // auto f = std::chrono::high_resolution_clock::now(); // profiling
        // subdurations[0] = f - s; // profiling
        // std::cout << "fetchData(): " << YELLOW << subdurations[0].count() << " seconds" << RESET; // profiling

        // s = std::chrono::high_resolution_clock::now();  // profiling
        ExchangeDataResponses responses = retrieveResponses(futures);
        // f = std::chrono::high_resolution_clock::now();  // profiling
        // subdurations[1] = f - s;  // profiling
        // std::cout << "retrieveResponses(): " << YELLOW << subdurations[1].count() << " seconds" << RESET;  // profiling

        // s = std::chrono::high_resolution_clock::now(); // profiling
        writeResponses(responses);
        // f = std::chrono::high_resolution_clock::now(); // profiling
        // subdurations[2] = f - s; // profiling
        // std::cout << "writeResponses(): " << YELLOW << subdurations[2].count() << " seconds" << RESET; // profiling

        // s = std::chrono::high_resolution_clock::now(); // profiling
        ExchangeDataVectors vectors = parseResponsesToVectors(responses);
        // f = std::chrono::high_resolution_clock::now(); // profiling
        // subdurations[3] = f - s; // profiling
        // std::cout << "parseResponsesToVectors(): " << YELLOW << subdurations[3].count() << " seconds" << RESET; // profiling

        // s = std::chrono::high_resolution_clock::now(); // profiling
        analyseBtcData(vectors);
        // f = std::chrono::high_resolution_clock::now(); // profiling
        // subdurations[4] = f - s; // profiling
        // std::cout << "analyseBtcData(): " << YELLOW << subdurations[4].count() << " seconds" << RESET; // profiling

        // s = std::chrono::high_resolution_clock::now(); // profiling
        analyseEthData(vectors);
        // f = std::chrono::high_resolution_clock::now(); // profiling
        // subdurations[5] = f - s; // profiling
        // std::cout << "analyseEthData(): " << YELLOW << subdurations[5].count() << " seconds" << RESET; // profiling

        // auto finish = std::chrono::high_resolution_clock::now(); // profiling
        // durations[i] = finish - start; // profiling
        // std::cout << GREEN << "Run-" << i << " completed in " << durations[i].count() << " seconds" << RESET; // profiling
    }
    freeMemory();
}
