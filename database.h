#pragma once

#include <memory>
#include <string>

namespace sql
{
    class Connection;
}

// Owns the one connection to the database. Every statement lives in a repository;
// this class only opens and closes the connection and hands it out.
class Database
{
private:
    std::unique_ptr<sql::Connection> conn;
    std::string lastError;

public:
    Database();
    ~Database();

    bool Connect(const std::string& user, const std::string& password);

    bool IsConnected() const;
    sql::Connection& GetConnection();

    std::string GetLastError() const;
};
