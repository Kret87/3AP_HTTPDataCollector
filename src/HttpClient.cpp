#include "HttpClient.h"

#include <curl/curl.h>
#include <stdexcept>

size_t HttpClient::writeCallback(void* contents,
                                 size_t size,
                                 size_t nmemb,
                                 void* userp)
{
    ((std::string*)userp)->append(
        (char*)contents,
        size * nmemb);

    return size * nmemb;
}

std::string HttpClient::get(const std::string& url,
                            const std::string& networkInterface)
{
    CURL* curl = curl_easy_init();

    if (!curl)
        throw std::runtime_error("Unable to initialize CURL.");

    std::string response;

    curl_easy_setopt(curl,
                     CURLOPT_URL,
                     url.c_str());

    curl_easy_setopt(curl,
                     CURLOPT_INTERFACE,
                     networkInterface.c_str());

    curl_easy_setopt(curl,
                 CURLOPT_CONNECTTIMEOUT,
                 3L);

    curl_easy_setopt(curl,
                 CURLOPT_TIMEOUT,
                 5L);

    curl_easy_setopt(curl,
                     CURLOPT_WRITEFUNCTION,
                     writeCallback);

    curl_easy_setopt(curl,
                     CURLOPT_WRITEDATA,
                     &response);

    CURLcode result = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
        throw std::runtime_error(curl_easy_strerror(result));

    return response;
}
