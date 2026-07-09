#pragma once

#include <pqxx/pqxx>
#include <string>

class Database
{
public:

    Database();

    bool connect();

    bool initialize();
    
    std::string getCurrentTimestamp();
    
    void setDeviceId(const std::string& deviceId);

    bool insert(const std::string& variable,
            double value,
            const std::string& sampleTime);

private:

    std::unique_ptr<pqxx::connection> connection_;
    std::string deviceId_;
    std::string tableName_;
};
