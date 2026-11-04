#include "CountryRepository.h"

#include "../database.h"

#include <mariadb/conncpp.hpp>

#include <memory>
#include <stdexcept>

CountryRepository::CountryRepository(Database& database)
    : RepositoryBase(database)
{
}

std::string CountryRepository::GetLastError() const
{
    return RepositoryBase::GetLastError();
}

std::optional<Country> CountryRepository::GetByID(int id)
{
    std::optional<Country> country;

    Run([&]
    {
        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement(
                "SELECT id, name FROM country WHERE id = ?"
            )
        );

        statement->setInt(1, id);

        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

        if (result->next())
        {
            Country found;
            found.id = result->getInt("id");
            found.name = result->getString("name");

            country = found;
        }
    });

    return country;
}

int CountryRepository::FindOrCreate(const std::string& name)
{
    int id = 0;

    Run([&]
    {
        if (name.empty())
        {
            throw std::runtime_error("Country is required.");
        }

        // One statement for both cases: an existing name keeps its own id.
        std::unique_ptr<sql::PreparedStatement> insert(
            Connection().prepareStatement(
                "INSERT INTO country (name) VALUES (?) "
                "ON DUPLICATE KEY UPDATE id = LAST_INSERT_ID(id)"
            )
        );

        insert->setString(1, name);
        insert->executeUpdate();

        std::unique_ptr<sql::Statement> statement(
            Connection().createStatement()
        );

        std::unique_ptr<sql::ResultSet> result(
            statement->executeQuery("SELECT LAST_INSERT_ID() AS id")
        );

        if (result->next())
        {
            id = result->getInt("id");
        }
    });

    return id;
}
