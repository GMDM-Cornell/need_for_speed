#include "database.h"

#include <mariadb/conncpp.hpp>

bool Database::Connect(const std::string& user, const std::string& password)
{
    try
    {
        sql::Driver* driver = sql::mariadb::get_driver_instance();

        sql::SQLString url("jdbc:mariadb://localhost:3306/need_for_speed");

        sql::Properties properties({
            {"user", user},
            {"password", password}
            });

        conn.reset(driver->connect(url, properties));

        if (!conn)
        {
            lastError = "Could not establish database connection.";
            return false;
        }

        lastError.clear();
        return true;
    }
    catch (sql::SQLException& e)
    {
        lastError = e.what();
        return false;
    }
}

Database::~Database() = default;

bool Database::IsConnected() const
{
    return conn != nullptr;
}

sql::Connection& Database::GetConnection()
{
    return *conn;
}

std::string Database::GetLastError() const
{
    return lastError;
}
