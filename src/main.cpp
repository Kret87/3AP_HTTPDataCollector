#include <iostream>
#include <string>
#include "Configuration.h"
#include "Database.h"
#include "HttpClient.h"
#include "PollScheduler.h"
#include "LicenseManager.h"

int main(int argc, char* argv[])
{
    std::cout << "=====================================\n";
    std::cout << " 3AP_HTTPDataCollector v0.8\n";
    std::cout << "=====================================\n\n";

    LicenseManager licenseManager;

    if (argc > 1)
    {
        if (std::string(argv[1]) == "--register"){
            if(licenseManager.registerDevice()){
                std::cout << "Device registered.\n";
                return true;
            }else{
                std::cout << "Device not registered.\n";
                return false;
            }
        }else if(std::string(argv[1]) == "--verify"){
            if(licenseManager.verifyLicense()){
                std::cout << "License ok.\n";
                return true;
            }else{
                std::cout << "License invalid.\n";
                return false;
            } 
        }
    }

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