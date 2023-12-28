#include "util/RestClient.h"

#include <iostream>
#include <stdexcept>

RestClient::RestClient() {}

RestClient::~RestClient() {}

size_t RestClient::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp)
{
    userp->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string RestClient::HTTP_GET(const std::string &url)
{
    std::string readBuffer;
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        throw std::runtime_error("Failed to initialize curl handle\n");
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    curl_easy_cleanup(curl);
    return readBuffer;
}