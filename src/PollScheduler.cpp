#include "PollScheduler.h"

#include <chrono>
#include <iostream>
#include <thread>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

void PollScheduler::run(const Configuration& configuration,
                        HttpClient& httpClient,
                        Database& database)
{
    const auto& endpoints = configuration.endpoints();

    lastPollTimes.resize(
        endpoints.size(),
        std::chrono::steady_clock::now());

    while (true)
    {
        auto now = std::chrono::steady_clock::now();

        for (std::size_t i = 0; i < endpoints.size(); ++i)
        {
            const auto& endpoint = endpoints[i];

            if (!endpoint.enabled)
                continue;

            auto elapsed =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    now - lastPollTimes[i]);

            if (elapsed.count() < endpoint.intervalMs)
                continue;

            try
            {
                std::cout
                    << "\nConnecting to: "
                    << endpoint.description
                    << '\n';

                std::string response =
                    httpClient.get(
                        endpoint.url,
                        endpoint.networkInterface);

                json j = json::parse(response);

                std::string deviceId =
                    j["device"]["serial_number"];

                std::cout << deviceId << '\n';

                database.setDeviceId(deviceId);
                database.initialize();

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
            catch (const std::exception& ex)
            {
                std::cerr
                    << endpoint.description
                    << ": "
                    << ex.what()
                    << '\n';
            }

            lastPollTimes[i] = now;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(100));
    }
}