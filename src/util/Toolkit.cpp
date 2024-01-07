#include "util/Toolkit.h"
#include "util/JsonProcessor.h"

#include <stdexcept>
#include <iostream>

std::string Toolkit::convertDeltaExpirationToDeribit(const std::string &dltExpiry)
{
    std::string months[] = {
        "JAN",
        "FEB",
        "MAR",
        "APR",
        "MAY",
        "JUN",
        "JUL",
        "AUG",
        "SEP",
        "OCT",
        "NOV",
        "DEC",
    };

    std::string dayStr = dltExpiry.substr(0, 2);
    std::string monthStr = dltExpiry.substr(2, 2);
    std::string yearStr = dltExpiry.substr(4, 2);

    int monthIndex = std::stoi(monthStr) - 1;
    if (monthIndex < 0 || monthIndex >= 12)
    {
        std::ostringstream oss;
        oss << "Invalid month index: " << monthIndex;
        throw std::out_of_range(oss.str());
    }

    std::string deribitExpiration = dayStr + months[monthIndex] + yearStr;
    return deribitExpiration;
}

void Toolkit::writeJsonToFile(const std::string &filename, const std::string &data)
{
    // Open file
    std::ofstream file;
    file.open(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: Unable to open file \"" + filename + "\" for writing.");
    }

    // Format data
    std::string formattedData;
    try
    {
        formattedData = JsonProcessor::formatJSON(data);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Error: Failed to format JSON data. " + std::string(e.what()));
    }

    // Write file
    file << formattedData;
    if (file.fail())
    {
        throw std::runtime_error("Error: Failed to write to the file \"" + filename + "\".");
    }

    // Close file
    file.close();
}

void Toolkit::sortOptionPairsByReturnPerc(std::vector<OptionPair> &optionPairs)
{
    std::sort(optionPairs.begin(), optionPairs.end(), [](const OptionPair &a, const OptionPair &b)
              {
                  return a.returnPerc > b.returnPerc; // Sort in descending order
              });
}

std::string Toolkit::toLower(const std::string &str)
{
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

std::string Toolkit::toUpper(const std::string &str)
{
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}