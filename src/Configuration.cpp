#include "Configuration.h"

#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool Configuration::load(const std::string& fileName)
{
    std::ifstream file(fileName);

    if (!file.is_open())
        return false;

    json j = json::parse(file);

    endpoints_.clear();

    for (const auto& endpointJson : j["endpoints"])
    {
        Endpoint endpoint;

        endpoint.description      = endpointJson["description"];
        endpoint.enabled          = endpointJson["enabled"];
        endpoint.networkInterface = endpointJson["interface"];
        endpoint.url              = endpointJson["url"];
        endpoint.intervalMs       = endpointJson["interval_ms"];

        endpoints_.push_back(endpoint);
    }

    return true;
}

const std::vector<Endpoint>& Configuration::endpoints() const
{
    return endpoints_;
}
