#ifndef TOOLKIT_H
#define TOOLKIT_H

#include "OptionPair.h"

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

class Toolkit
{
public:
    Toolkit();
    ~Toolkit();
    static std::string convertDeltaExpirationToDeribit(const std::string &deltaExpiration);
    static void writeJsonToFile(const std::string &filename, const std::string &data);
    static void sortOptionPairsByReturnPerc(std::vector<OptionPair> &optionPairs);
    static std::string toLower(const std::string &str);
    static std::string toUpper(const std::string &str);
};

#endif // TOOLKIT_H