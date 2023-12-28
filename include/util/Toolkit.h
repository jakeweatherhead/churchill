#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <string>
#include <sstream>
#include <fstream>

class Toolkit
{
public:
    Toolkit();
    ~Toolkit();
    static std::string convertDeltaExpirationToDeribit(const std::string &deltaExpiration);
    static void writeToFile(const std::string &filename, const std::string &data);
};

#endif // TOOLKIT_H