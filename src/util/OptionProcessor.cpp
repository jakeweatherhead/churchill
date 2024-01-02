#include "util/OptionProcessor.h"
#include "util/Toolkit.h"

#include "exchange/DeltaExchangeManager.h"
#include "exchange/DeribitExchangeManager.h"

#include <iostream>

OptionProcessor::OptionProcessor() {}

OptionProcessor::~OptionProcessor() {}

std::vector<OptionPair> OptionProcessor::createOptionPairs(
    const std::string &symbol,
    std::vector<DeribitOption> &deribitOptions,
    std::vector<DeltaOption> &deltaOptions,
    std::vector<DeribitFutures> &deribitFutures)
{
    std::vector<OptionPair> candidates;
    for (int i = 0; i < deltaOptions.size(); i++)
    {
        // Parse delta option data
        std::string deltaSymbol = deltaOptions[i].symbol;
        std::vector<std::string> deltaSymbolParts = DeltaExchangeManager::split(deltaSymbol, '-');
        std::string deltaOptionType = deltaSymbolParts[0];
        std::string ticker = deltaSymbolParts[1];
        std::string strikeStr = deltaSymbolParts[2];
        std::string deltaExpiration = deltaSymbolParts[3];

        // Construct matching deribit data
        std::string deribitOptionType = deltaOptionType == "C" ? "P" : "C";
        double strike = std::stod(strikeStr);
        std::string deribitExpiration = Toolkit::convertDeltaExpirationToDeribit(deltaExpiration);

        // Retrieve matching deribit futures contract
        std::string futuresInstrumentName = ticker + "-" + deribitExpiration;
        DeribitFutures futuresContract;
        try
        {
            futuresContract = findDeribitFutures(deribitFutures, futuresInstrumentName);
        }
        catch (const std::exception &e)
        {
            continue;
        }

        // Retrieve matching deribit option contract
        DeribitOption deribitComplement;
        std::string deribitEqvSymbol = ticker + "-" + deribitExpiration + "-" + strikeStr + "-" + deribitOptionType;
        try
        {
            deribitComplement = DeribitExchangeManager::findDeribitComplement(deribitOptions, deribitEqvSymbol);
        }
        catch (const std::exception &e)
        {
            continue;
        }

        // Calculate profit components
        double callPrice =
            deltaOptionType == "C"
                ? std::stod(deltaOptions[i].quotes.best_bid)
                : deribitComplement.bid_price;

        double putPrice =
            deltaOptionType == "C"
                ? deribitComplement.ask_price
                : std::stod(deltaOptions[i].quotes.best_bid);

        double makerFee = 0.0003 * callPrice;
        double takerFee = 0.0003 * putPrice;

        // Parse quote_currencies
        std::string callCurrency = deltaOptionType == "C" ? "USDT" : symbol;
        std::string putCurrency = deltaOptionType == "P" ? "USDT" : symbol;

        if (callCurrency == symbol)
        {
            callPrice *= deribitComplement.underlying_price;
            callCurrency = "USDT";
        }
        else
        {
            putPrice *= deribitComplement.underlying_price;
            putCurrency = "USDT";
        }

        OptionPair optionPair = {
            deltaSymbol,
            deribitEqvSymbol,
            callPrice,
            callCurrency,
            makerFee,
            strike,
            putPrice,
            putCurrency,
            takerFee,
            futuresContract.mid_price,
            0.0, // capitalRequired
            0.0, // profit
            0.0, // returnPerc
        };
        candidates.push_back(optionPair);
    }
    return candidates;
}

DeribitFutures OptionProcessor::findDeribitFutures(const std::vector<DeribitFutures> &deribitFutures, const std::string &futuresInstrumentName)
{
    for (int i = 0; i < deribitFutures.size(); i++)
    {
        if (deribitFutures[i].instrument_name == futuresInstrumentName)
        {
            return deribitFutures[i];
        }
    }
    throw std::runtime_error("Could not find deribit futures contract: " + futuresInstrumentName + "\n");
}
