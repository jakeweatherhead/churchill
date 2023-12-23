#include "util/RestClient.h"

#include <iostream>

RestClient::RestClient()
{
  curl = curl_easy_init();
}

RestClient::~RestClient()
{
  if (curl)
  {
    curl_easy_cleanup(curl);
  }
}

size_t RestClient::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp)
{
  userp->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string RestClient::HTTP_GET(const std::string &url)
{
  std::string readBuffer;
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
  }
  return readBuffer;
}

// TODO: Implement HTTP_POST


