#include "CityRepository.h"

#include "../database.h"

#include <mariadb/conncpp.hpp>

#include <memory>
#include <stdexcept>

CityRepository::CityRepository(Database& database)
    : RepositoryBase(database)
{
}

std::string CityRepository::GetLastError() const
{
    return RepositoryBase::GetLastError();
}

std::optional<City> CityRepository::GetByID(int id)
{
    std::optional<City> city;

    Run([&]
    {
        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement(
                "SELECT id, country_id, name FROM city WHERE id = ?"
            )
        );

        statement->setInt(1, id);

        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

        if (result->next())
        {
            City found;
            found.id = result->getInt("id");
            found.countryID = result->getInt("country_id");
            found.name = result->getString("name");

            city = found;
        }
    });

    return city;
}

int CityRepository::FindOrCreate(int countryID, const std::string& name)
{
    int id = 0;

    Run([&]
    {
        if (name.empty())
        {
            throw std::runtime_error("City is required.");
        }

        if (countryID == 0)
        {
            throw std::runtime_error("Country is required.");
        }

        std::unique_ptr<sql::PreparedStatement> insert(
            Connection().prepareStatement(
                "INSERT INTO city (country_id, name) VALUES (?, ?) "
                "ON DUPLICATE KEY UPDATE id = LAST_INSERT_ID(id)"
            )
        );

        insert->setInt(1, countryID);
        insert->setString(2, name);
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
