#include "Database.h"
#include <iostream>
#include <algorithm>
#include <cctype>

Database::Database()
    : connection_(nullptr)
{
}

bool Database::connect()
{
    try
    {
        connection_ = std::make_unique<pqxx::connection>(
            "host=localhost "
            "port=5432 "
            "dbname=collector "
            "user=collector "
            "password=collector");

        if (connection_->is_open())
        {
            std::cout << "Connected to PostgreSQL.\n";
            return true;
        }

        return false;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Database error: "
                  << ex.what()
                  << '\n';

        return false;
    }
}

bool Database::initialize()
{
    try
    {
        pqxx::work transaction(*connection_);

	std::string sql =
    	"CREATE TABLE IF NOT EXISTS " + tableName_ + R"(
	(
   	 id BIGSERIAL PRIMARY KEY,

    	sample_time TIMESTAMPTZ NOT NULL,

    	variable TEXT NOT NULL,

    	value DOUBLE PRECISION NOT NULL
	);
	)";

transaction.exec(sql);

        transaction.commit();

        std::cout << "Telemetry table ready.\n";

        return true;
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';

        return false;
    }
}

std::string Database::getCurrentTimestamp()
{
    pqxx::work transaction(*connection_);

pqxx::result result =
    transaction.exec(
        "SELECT CURRENT_TIMESTAMP::text");

std::string timestamp =
    result[0][0].as<std::string>();

    transaction.commit();

    return timestamp;
}

void Database::setDeviceId(const std::string& deviceId)
{
    deviceId_ = deviceId;

    tableName_ = "telemetry_";

    for (char c : deviceId_)
    {
        if (std::isalnum(static_cast<unsigned char>(c)))
            tableName_ += c;
        else
            tableName_ += '_';
    }
}

bool Database::insert(const std::string& variable,
                      double value,
                      const std::string& sampleTime)
{
    try
    {
        pqxx::work transaction(*connection_);

        std::string sql =
            "INSERT INTO " + tableName_ + R"(
(
    sample_time,
    variable,
    value
)
VALUES
(
    $1,
    $2,
    $3
);
)";

        transaction.exec_params(
            sql,
            sampleTime,
            variable,
            value);

        transaction.commit();

        return true;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Insert error: "
                  << ex.what()
                  << '\n';

        return false;
    }
}
