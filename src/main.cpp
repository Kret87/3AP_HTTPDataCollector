#include <iostream>

#include "Configuration.h"
#include "Database.h"
#include "HttpClient.h"
#include "PollScheduler.h"

int main()
{
    std::cout << "=====================================\n";
    std::cout << " 3AP_HTTPDataCollector v0.8\n";
    std::cout << "=====================================\n\n";

    try
    {
        Configuration configuration;

        if (!configuration.load("../config/collector.json"))
        {
            std::cerr << "Unable to load configuration.\n";
            return 1;
        }

        HttpClient http;
        Database database;

        database.connect();

        PollScheduler scheduler;
        scheduler.run(
            configuration,
            http,
            database);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "ERROR: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}