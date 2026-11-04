#include "BrandRepository.h"

#include "ICityRepository.h"
#include "ICountryRepository.h"
#include "../database.h"

#include <mariadb/conncpp.hpp>

#include <istream>
#include <iterator>
#include <memory>
#include <stdexcept>

BrandRepository::BrandRepository(
    Database& database,
    ICityRepository& cityRepository,
    ICountryRepository& countryRepository)
    : RepositoryBase(database),
      cityRepository(cityRepository),
      countryRepository(countryRepository)
{
}

std::string BrandRepository::GetLastError() const
{
    return RepositoryBase::GetLastError();
}

std::vector<Brand> BrandRepository::GetAll()
{
    std::vector<Brand> brands;

    Run([&]
    {
        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement(
                "SELECT id, city_id, name, logo FROM brand ORDER BY name"
            )
        );

        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

        while (result->next())
        {
            Brand brand;

            brand.id = result->getInt("id");
            brand.name = result->getString("name");

            if (!result->isNull("city_id"))
            {
                brand.cityID = result->getInt("city_id");
            }

            if (!result->isNull("logo"))
            {
                std::unique_ptr<std::istream> blob(result->getBlob("logo"));

                brand.logo.assign(
                    std::istreambuf_iterator<char>(*blob),
                    std::istreambuf_iterator<char>()
                );
            }

            brands.push_back(brand);
        }
    });

    return brands;
}

std::optional<Brand> BrandRepository::GetByID(int brandID)
{
    std::optional<Brand> brand;

    Run([&]
    {
        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement(
                "SELECT id, city_id, name, logo FROM brand WHERE id = ?"
            )
        );

        statement->setInt(1, brandID);

        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

        if (result->next())
        {
            Brand found;

            found.id = result->getInt("id");
            found.name = result->getString("name");

            if (!result->isNull("city_id"))
            {
                found.cityID = result->getInt("city_id");
            }

            if (!result->isNull("logo"))
            {
                std::unique_ptr<std::istream> blob(result->getBlob("logo"));

                found.logo.assign(
                    std::istreambuf_iterator<char>(*blob),
                    std::istreambuf_iterator<char>()
                );
            }

            brand = found;
        }
    });

    return brand;
}

bool BrandRepository::SetHeadquarters(
    int brandID,
    const std::string& city,
    const std::string& country)
{
    try
    {
        lastError.clear();

        if (city.empty() || country.empty())
        {
            throw std::runtime_error("Country and city are required.");
        }

        sql::Connection& connection = Connection();

        // Country, city and the brand row change together or not at all.
        connection.setAutoCommit(false);

        int countryID = countryRepository.FindOrCreate(country);

        if (countryID == 0)
        {
            throw std::runtime_error(countryRepository.GetLastError());
        }

        int cityID = cityRepository.FindOrCreate(countryID, city);

        if (cityID == 0)
        {
            throw std::runtime_error(cityRepository.GetLastError());
        }

        std::unique_ptr<sql::PreparedStatement> statement(
            connection.prepareStatement(
                "UPDATE brand SET city_id = ? WHERE id = ?"
            )
        );

        statement->setInt(1, cityID);
        statement->setInt(2, brandID);

        // MariaDB reports changed rows, so 0 rows can also mean "nothing changed".
        if (statement->executeUpdate() == 0 && !Exists("brand", brandID))
        {
            throw std::runtime_error("Brand not found.");
        }

        connection.commit();
        connection.setAutoCommit(true);

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

    try
    {
        Connection().rollback();
        Connection().setAutoCommit(true);
    }
    catch (...)
    {
    }

    return false;
}
