#pragma once

#include <string>

struct Endpoint
{
    std::string description;

    bool enabled;

    std::string networkInterface;

    std::string url;

    int intervalMs;
};
