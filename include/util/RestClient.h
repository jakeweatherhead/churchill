#ifndef REST_CLIENT_H
#define REST_CLIENT_H

#include <string>
#include <curl/curl.h>

class RestClient
{
public:
  RestClient();
  ~RestClient();
  static std::string HTTP_GET(const std::string &url);
  void postData();

private:
  std::string url;
  CURL *curl;

  /**
   * @brief Callback function for handling data received from a libcurl request.
   *
   * This function is called by libcurl as part of the data transfer process.
   * It is used to append the received data to a std::string. The function
   * is specified as the callback for CURLOPT_WRITEFUNCTION when setting up
   * the libcurl options.
   *
   * @param contents Pointer to the received data.
   * @param size Size of the data elements received.
   * @param nmemb Number of data elements received.
   * @param userp User-defined parameter; in this case, a pointer to a std::string
   *              where the received data is appended.
   * @return The total number of bytes processed by the callback. Returning a
   *         number different from size * nmemb signals an error to libcurl
   *         and causes the data transfer to stop.
   */
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp);
};

#endif // REST_CLIENT_H
