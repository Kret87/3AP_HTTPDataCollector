#pragma once

#include <string>

class HttpClient
{
public:
    std::string get(
    const std::string& url,
    const std::string& networkInterface);

private:
    static size_t writeCallback(void* contents,
                                size_t size,
                                size_t nmemb,
                                void* userp);
};
