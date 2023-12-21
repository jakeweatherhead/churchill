#include <iostream>
#include <curl/curl.h>
#include <jansson.h>

// Callback function to receive the data
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp)
{
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to make the REST API call
void makeAPICall()
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=BTC&kind=future");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK) 
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } 
    else 
    {
        json_error_t error;
        json_t *root = json_loads(readBuffer.c_str(), 0, &error);
        if (!root)
        {
          std::cerr << "Error parsing JSON: " << error.text << std::endl;
        } 
        else 
        {
            char *formatted = json_dumps(root, JSON_INDENT(4));
            if (!formatted)
            {
              std::cerr << "Error printing JSON" << std::endl;
            }
            else
            {
              std::cout << formatted << std::endl;
              free(formatted);
            }
            json_decref(root);
        }
    }
    curl_easy_cleanup(curl);  // Clean up
}

int main()
{
    makeAPICall();
    return 0;
}
