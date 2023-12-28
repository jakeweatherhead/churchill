#include "util/Toolkit.h"
#include "util/JsonProcessor.h"

#include <stdexcept>

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

void Toolkit::writeToFile(const std::string &filename, const std::string &data)
{
    std::ofstream file;
    file.open(filename);
    std::string formattedData = JsonProcessor::formatJSON(data);
    file << formattedData;
    file.close();
}