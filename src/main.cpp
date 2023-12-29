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

const int NUM_RUNS = 10;
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
    Toolkit::writeToFile("data/pcp_0/res/dbt_btc_futures_res.json", responses.dbtBtcFuturesResponse);
    Toolkit::writeToFile("data/pcp_0/res/dbt_btc_options_res.json", responses.dbtBtcOptionsResponse);
    Toolkit::writeToFile("data/pcp_0/res/dbt_eth_futures_res.json", responses.dbtEthFuturesResponse);
    Toolkit::writeToFile("data/pcp_0/res/dbt_eth_options_res.json", responses.dbtEthOptionsResponse);
    Toolkit::writeToFile("data/pcp_0/res/dlt_call_options_res.json", responses.dltCallOptionsResponse);
    Toolkit::writeToFile("data/pcp_0/res/dlt_put_options_res.json", responses.dltPutOptionsResponse);
}

ExchangeDataVectors parseResponsesToVectors(ExchangeDataResponses &responses)
{
    ExchangeDataVectors vectors;
    try
    {
        vectors.dbtBtcFuturesVec = dbtEM->parseFuturesToVector(responses.dbtBtcFuturesResponse);
        vectors.dbtBtcOptionsVec = dbtEM->parseOptionsToVector(responses.dbtBtcOptionsResponse);
        vectors.dbtEthFuturesVec = dbtEM->parseFuturesToVector(responses.dbtEthFuturesResponse);
        vectors.dbtEthOptionsVec = dbtEM->parseOptionsToVector(responses.dbtEthOptionsResponse);

        vectors.dltBtcCallOptionsVec = dltEM->parseOptionsToVector("BTC", responses.dltCallOptionsResponse);
        vectors.dltBtcPutOptionsVec = dltEM->parseOptionsToVector("BTC", responses.dltPutOptionsResponse);
        vectors.dltEthCallOptionsVec = dltEM->parseOptionsToVector("ETH", responses.dltCallOptionsResponse);
        vectors.dltEthPutOptionsVec = dltEM->parseOptionsToVector("ETH", responses.dltPutOptionsResponse);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in parseResponsesToVectors: " << e.what() << std::endl;
    }
    return vectors;
}

void analyseBtcData(ExchangeDataVectors &vectors)
{
    std::vector<DeltaOption> dltOptionsVec = vectors.dltBtcCallOptionsVec;
    dltOptionsVec.insert(dltOptionsVec.end(), vectors.dltBtcPutOptionsVec.begin(), vectors.dltBtcPutOptionsVec.end());
    btcCandidates = optionProcessor->createOptionPairs("BTC", vectors.dbtBtcOptionsVec, dltOptionsVec, vectors.dbtBtcFuturesVec);
    pcpStrategy_0->filterArbitrageOpportunities(btcCandidates);

    if (btcCandidates.size() == 0)
    {
        std::cout << "No BTC arbitrage opportunities found." << std::endl;
    }
    else
    {
        Toolkit::sortOptionPairsByReturnPerc(btcCandidates);
        std::string candidates = JsonProcessor::convertOptionPairsToString(btcCandidates);
        std::string filename = "data/pcp_0/arb/BTC_ops.json";
        Toolkit::writeToFile(filename, candidates);
    }
}

void analyseEthData(ExchangeDataVectors &vectors)
{
    std::vector<DeltaOption> dltOptionsVec = vectors.dltEthCallOptionsVec;
    dltOptionsVec.insert(dltOptionsVec.end(), vectors.dltEthPutOptionsVec.begin(), vectors.dltEthPutOptionsVec.end());
    ethCandidates = optionProcessor->createOptionPairs("ETH", vectors.dbtEthOptionsVec, dltOptionsVec, vectors.dbtEthFuturesVec);
    pcpStrategy_0->filterArbitrageOpportunities(ethCandidates);

    if (ethCandidates.size() == 0)
    {
        std::cout << "No ETH arbitrage opportunities found." << std::endl;
    }
    else
    {
        Toolkit::sortOptionPairsByReturnPerc(ethCandidates);
        std::string candidates = JsonProcessor::convertOptionPairsToString(ethCandidates);
        std::string filename = "data/pcp_0/arb/ETH_ops.json";
        Toolkit::writeToFile(filename, candidates);
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

        std::cout << "-----------------------------------------------\n";
        auto s = std::chrono::high_resolution_clock::now(); // <-- start of full execution

        auto start = std::chrono::high_resolution_clock::now();
        ExchangeDataFutures futures = fetchData(); // <--
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "Data fetched in " << elapsed.count() << " seconds" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        ExchangeDataResponses responses = retrieveResponses(futures); // <--
        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
        std::cout << "Data retrieved in " << elapsed.count() << " seconds" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        writeResponses(responses); // <--
        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
        std::cout << "Data written in " << elapsed.count() << " seconds" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        ExchangeDataVectors vectors = parseResponsesToVectors(responses); // <--
        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
        std::cout << "Data parsed in " << elapsed.count() << " seconds" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        analyseBtcData(vectors); // <--
        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
        std::cout << "BTC data analysed in " << elapsed.count() << " seconds" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        analyseEthData(vectors); // <--
        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
        std::cout << "ETH data analysed in " << elapsed.count() << " seconds" << std::endl;

        auto f = std::chrono::high_resolution_clock::now(); // <-- end of full execution
        durations[i] = f - s;
        std::cout << "Run-" << i + 1 << " completed: " << durations[i].count() << " seconds" << std::endl;
    }
    freeMemory();
}
