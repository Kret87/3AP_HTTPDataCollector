#pragma once

#include <chrono>
#include <vector>

#include "Configuration.h"
#include "Database.h"
#include "HttpClient.h"

class PollScheduler
{
public:
    void run(const Configuration& configuration,
             HttpClient& httpClient,
             Database& database);

private:
    std::vector<std::chrono::steady_clock::time_point> lastPollTimes;
};