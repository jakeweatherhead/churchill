#ifndef REST_CLIENT_H
#define REST_CLIENT_H

#include <string>
#include <curl/curl.h>

class RestClient
{
public:
    RestClient(const std::string &url);
    ~RestClient();
    std::string fetchData();

private:
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp);
    std::string url;
    CURL *curl;
};

#endif // REST_CLIENT_H
