#include "PollScheduler.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace
{
    void decodeHtmlEntities(std::string& text)
    {
        std::size_t pos = 0;

        while ((pos = text.find("&#x2d;", pos)) != std::string::npos)
        {
            text.replace(pos, 6, "-");
            ++pos;
        }
    }
}

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

                decodeHtmlEntities(response);

                json j = json::parse(response);

                std::string deviceId =
                    j["device"]["serial_number"];

                std::cout << deviceId << '\n';

                database.setDeviceId(deviceId);
                database.initialize();

                std::string sampleTime =
                    database.getCurrentTimestamp();

                std::vector<double> values;

                for (const auto& value : j["values"])
                {
                    double number = value.get<double>();

                    std::cout
                        << number
                        << '\n';

                    values.push_back(number);
                }

                database.insert(values, sampleTime);
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