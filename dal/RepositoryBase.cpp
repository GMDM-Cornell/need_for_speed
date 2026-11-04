#include "RepositoryBase.h"

#include "../database.h"

#include <mariadb/conncpp.hpp>

#include <memory>
#include <stdexcept>

RepositoryBase::RepositoryBase(Database& database)
    : database(database)
{
}

std::string RepositoryBase::GetLastError() const
{
    return lastError;
}

sql::Connection& RepositoryBase::Connection()
{
    if (!database.IsConnected())
    {
        throw std::runtime_error("Database is not connected.");
    }

    return database.GetConnection();
}

bool RepositoryBase::Run(const std::function<void()>& work)
{
    try
    {
        lastError.clear();
        work();
        return true;
    }
    catch (sql::SQLException& e)
    {
        lastError = e.what();
    }
    catch (const std::exception& e)
    {
        lastError = e.what();
    }

    return false;
}

bool RepositoryBase::Exists(const char* table, int id)
{
    std::unique_ptr<sql::PreparedStatement> statement(
        Connection().prepareStatement(
            std::string("SELECT id FROM ") + table + " WHERE id = ?"
        )
    );

    statement->setInt(1, id);

    std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

    return result->next();
}
