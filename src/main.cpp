#include <iostream>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>

#include "Configuration.h"
#include "Database.h"
#include "HttpClient.h"

using json = nlohmann::json;

int main()
{
    std::cout << "=====================================\n";
    std::cout << " 3AP_HTTPDataCollector v0.1\n";
    std::cout << "=====================================\n\n";

    try
    {
        HttpClient http;
        Database database;
        Configuration configuration;

        if (!configuration.load("../config/collector.json"))
        {
            std::cerr << "Unable to load configuration.\n";
            return 1;
        }

        database.connect();

        for (const auto& endpoint : configuration.endpoints())
        {
            if (!endpoint.enabled)
                continue;

            std::cout
                << "\nConnecting to: "
                << endpoint.description
                << std::endl;

            std::string response =
                http.get(
                    endpoint.url,
                    endpoint.networkInterface);

            json j = json::parse(response);

            std::string deviceId = j["device"]["serial_number"];

            std::cout << deviceId << '\n';

            database.setDeviceId(deviceId);
            database.initialize();

            std::cout << "\nValues\n";
            std::cout << "------\n\n";

            std::string sampleTime =
                database.getCurrentTimestamp();

            for (auto& variable : j["values"].items())
            {
                double value = variable.value();

                std::cout
                    << variable.key()
                    << " = "
                    << value
                    << '\n';

                database.insert(
                    variable.key(),
                    value,
                    sampleTime);
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "ERROR: " << ex.what() << '\n';
    }

    return 0;
}
