#pragma once

#include <functional>
#include <string>

namespace sql
{
    class Connection;
}

class Database;

// Shared plumbing for every repository: holds the connection, runs one piece of
// database work and keeps the error message instead of throwing into the GUI.
class RepositoryBase
{
public:
    std::string GetLastError() const;

protected:
    explicit RepositoryBase(Database& database);

    sql::Connection& Connection();
    bool Run(const std::function<void()>& work);
    bool Exists(const char* table, int id);

    std::string lastError;

private:
    Database& database;
};
