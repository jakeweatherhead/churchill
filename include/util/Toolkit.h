#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <string>
#include <sstream>

class Toolkit
{
public:
    Toolkit();
    ~Toolkit();
    static std::string convertDeltaExpirationToDeribit(const std::string &deltaExpiration);
};

#endif // TOOLKIT_H