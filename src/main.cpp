#include "RestClient.h"
#include "JsonProcessor.h"
#include <iostream>

int main()
{
    RestClient client("https://www.deribit.com/api/v2/public/get_book_summary_by_currency?currency=BTC&kind=future");
    std::string apiResponse = client.fetchData();
    std::string formattedJson = JsonProcessor::formatJson(apiResponse);
    std::cout << formattedJson << std::endl;
    return 0;
}
