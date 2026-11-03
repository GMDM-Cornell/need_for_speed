#include "Database.h"

#include <mariadb/conncpp.hpp>

#include <stdexcept>
#include <iterator>
#include <cstdint>
#include <istream>

Database::Database() = default;

Database::~Database() = default;

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

std::string Database::GetLastError() const
{
    return lastError;
}

std::vector<Brand> Database::GetBrands()
{
    std::vector<Brand> brands;

    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "SELECT id, name FROM brand ORDER BY name"
            )
        );

        std::unique_ptr<sql::ResultSet> result(
            statement->executeQuery()
        );

        while (result->next())
        {
            Brand brand;

            brand.id = result->getInt("id");
            brand.name = result->getString("name");

            brands.push_back(brand);
        }
    }
    catch (sql::SQLException& e)
    {
        lastError = e.what();
    }
    catch (const std::exception& e)
    {
        lastError = e.what();
    }

    return brands;
}

std::vector<std::uint8_t> Database::GetBrandLogo(int brandID)
{
    std::vector<std::uint8_t> logo;

    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "SELECT logo FROM brand WHERE id = ?"
            )
        );

        statement->setInt(1, brandID);

        std::unique_ptr<sql::ResultSet> result(
            statement->executeQuery()
        );

        if (result->next())
        {
            std::unique_ptr<std::istream> blob(result->getBlob("logo"));

            if (blob)
            {
                logo.assign(
                    std::istreambuf_iterator<char>(*blob),
                    std::istreambuf_iterator<char>()
                );
            }
        }
    }
    catch (sql::SQLException& e)
    {
        lastError = e.what();
    }
    catch (const std::exception& e)
    {
        lastError = e.what();
    }

    return logo;
}

std::vector<VehicleType> Database::GetTypes(int brandID)
{
    std::vector<VehicleType> types;

    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "SELECT id, brand_id, name "
                "FROM vehicle_type "
                "WHERE brand_id = ? "
                "ORDER BY name"
            )
        );

        statement->setInt(1, brandID);

        std::unique_ptr<sql::ResultSet> result(
            statement->executeQuery()
        );

        while (result->next())
        {
            VehicleType type;

            type.id = result->getInt("id");
            type.brandID = result->getInt("brand_id");
            type.name = result->getString("name");

            types.push_back(type);
        }
    }
    catch (sql::SQLException& e)
    {
        lastError = e.what();
    }
    catch (const std::exception& e)
    {
        lastError = e.what();
    }

    return types;
}

std::vector<CarDetails> Database::GetCarDetails(int typeID)
{
    std::vector<CarDetails> details;

    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "SELECT m.id AS model_id, "
                "m.vehicle_type_id, "
                "vt.name AS vehicle_type_name, "
                "m.name AS model_name, "
                "m.power_ps "
                "FROM model m "
                "JOIN vehicle_type vt "
                "ON m.vehicle_type_id = vt.id "
                "WHERE m.vehicle_type_id = ? "
                "ORDER BY m.name"
            )
        );

        statement->setInt(1, typeID);

        std::unique_ptr<sql::ResultSet> result(
            statement->executeQuery()
        );

        while (result->next())
        {
            CarDetails car;

            car.modelID = result->getInt("model_id");
            car.vehicleTypeID = result->getInt("vehicle_type_id");
            car.vehicleTypeName = result->getString("vehicle_type_name");
            car.modelName = result->getString("model_name");
            car.powerPS = result->getInt("power_ps");

            details.push_back(car);
        }
    }
    catch (sql::SQLException& e)
    {
        lastError = e.what();
    }
    catch (const std::exception& e)
    {
        lastError = e.what();
    }

    return details;
}

BrandLocation Database::GetBrandLocation(int brandID)
{
    BrandLocation location{};

    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "SELECT co.id AS country_id, "
                "co.name AS country_name, "
                "c.id AS city_id, "
                "c.name AS city_name "
                "FROM brand b "
                "JOIN city c ON b.city_id = c.id "
                "JOIN country co ON c.country_id = co.id "
                "WHERE b.id = ?"
            )
        );

        statement->setInt(1, brandID);

        std::unique_ptr<sql::ResultSet> result(
            statement->executeQuery()
        );

        if (result->next())
        {
            location.countryID = result->getInt("country_id");
            location.country = result->getString("country_name");
            location.cityID = result->getInt("city_id");
            location.city = result->getString("city_name");
        }
        else
        {
            lastError = "Brand location not found.";
        }
    }
    catch (sql::SQLException& e)
    {
        lastError = e.what();
    }
    catch (const std::exception& e)
    {
        lastError = e.what();
    }

    return location;
}

int Database::GetOrCreateCountryID(const std::string& country)
{
    std::unique_ptr<sql::PreparedStatement> select(
        conn->prepareStatement(
            "SELECT id FROM country WHERE name = ? LIMIT 1"
        )
    );

    select->setString(1, country);

    std::unique_ptr<sql::ResultSet> result(
        select->executeQuery()
    );

    if (result->next())
        return result->getInt("id");

    std::unique_ptr<sql::PreparedStatement> insert(
        conn->prepareStatement(
            "INSERT INTO country (name) VALUES (?)"
        )
    );

    insert->setString(1, country);
    insert->executeUpdate();

    std::unique_ptr<sql::Statement> statement(
        conn->createStatement()
    );

    std::unique_ptr<sql::ResultSet> inserted(
        statement->executeQuery("SELECT LAST_INSERT_ID() AS id")
    );

    if (inserted->next())
        return inserted->getInt("id");

    throw std::runtime_error("Could not create country.");
}

int Database::GetOrCreateCityID(const std::string& city, int countryID)
{
    std::unique_ptr<sql::PreparedStatement> select(
        conn->prepareStatement(
            "SELECT id FROM city "
            "WHERE name = ? AND country_id = ? LIMIT 1"
        )
    );

    select->setString(1, city);
    select->setInt(2, countryID);

    std::unique_ptr<sql::ResultSet> result(
        select->executeQuery()
    );

    if (result->next())
        return result->getInt("id");

    std::unique_ptr<sql::PreparedStatement> insert(
        conn->prepareStatement(
            "INSERT INTO city (country_id, name) VALUES (?, ?)"
        )
    );

    insert->setInt(1, countryID);
    insert->setString(2, city);
    insert->executeUpdate();

    std::unique_ptr<sql::Statement> statement(
        conn->createStatement()
    );

    std::unique_ptr<sql::ResultSet> inserted(
        statement->executeQuery("SELECT LAST_INSERT_ID() AS id")
    );

    if (inserted->next())
        return inserted->getInt("id");

    throw std::runtime_error("Could not create city.");
}

bool Database::UpdateBrandLocation(
    int brandID,
    const std::string& country,
    const std::string& city)
{
    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        if (country.empty() || city.empty())
            throw std::runtime_error("Country and city are required.");

        conn->setAutoCommit(false);

        int countryID = GetOrCreateCountryID(country);
        int cityID = GetOrCreateCityID(city, countryID);

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "UPDATE brand SET city_id = ? WHERE id = ?"
            )
        );

        statement->setInt(1, cityID);
        statement->setInt(2, brandID);

        int affectedRows = statement->executeUpdate();

        if (affectedRows == 0)
        {
            std::unique_ptr<sql::PreparedStatement> check(
                conn->prepareStatement(
                    "SELECT id FROM brand WHERE id = ?"
                )
            );

            check->setInt(1, brandID);

            std::unique_ptr<sql::ResultSet> result(
                check->executeQuery()
            );

            if (!result->next())
                throw std::runtime_error("Brand not found.");
        }

        conn->commit();
        conn->setAutoCommit(true);

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

    if (conn)
    {
        try
        {
            conn->rollback();
            conn->setAutoCommit(true);
        }
        catch (...)
        {
        }
    }

    return false;
}

bool Database::UpdateVehicleType(int vehicleTypeID, const std::string& name)
{
    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        if (name.empty())
            throw std::runtime_error("Vehicle type name is required.");

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "UPDATE vehicle_type "
                "SET name = ? "
                "WHERE id = ?"
            )
        );

        statement->setString(1, name);
        statement->setInt(2, vehicleTypeID);

        if (statement->executeUpdate() == 0)
            throw std::runtime_error("Vehicle type not found or no changes were made.");

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

bool Database::UpdateModel(int modelID, const std::string& name, int powerPS)
{
    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        if (powerPS < 0)
            throw std::runtime_error("Horsepower cannot be negative.");

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "UPDATE model "
                "SET name = NULLIF(?, ''), power_ps = ? "
                "WHERE id = ?"
            )
        );

        statement->setString(1, name);
        statement->setInt(2, powerPS);
        statement->setInt(3, modelID);

        int affectedRows = statement->executeUpdate();

        if (affectedRows == 0)
        {
            std::unique_ptr<sql::PreparedStatement> check(
                conn->prepareStatement(
                    "SELECT id FROM model WHERE id = ?"
                )
            );

            check->setInt(1, modelID);

            std::unique_ptr<sql::ResultSet> result(
                check->executeQuery()
            );

            if (!result->next())
                throw std::runtime_error("Model not found.");
        }

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

bool Database::UpdateCarType(const CarDetails& details)
{
    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        if (details.vehicleTypeName.empty())
            throw std::runtime_error("Vehicle type name is required.");

        if (details.powerPS < 0)
            throw std::runtime_error("Horsepower cannot be negative.");

        conn->setAutoCommit(false);

        std::unique_ptr<sql::PreparedStatement> typeStatement(
            conn->prepareStatement(
                "UPDATE vehicle_type "
                "SET name = ? "
                "WHERE id = ?"
            )
        );

        typeStatement->setString(1, details.vehicleTypeName);
        typeStatement->setInt(2, details.vehicleTypeID);
        typeStatement->executeUpdate();

        std::unique_ptr<sql::PreparedStatement> modelStatement(
            conn->prepareStatement(
                "UPDATE model "
                "SET name = NULLIF(?, ''), power_ps = ? "
                "WHERE id = ? AND vehicle_type_id = ?"
            )
        );

        modelStatement->setString(1, details.modelName);
        modelStatement->setInt(2, details.powerPS);
        modelStatement->setInt(3, details.modelID);
        modelStatement->setInt(4, details.vehicleTypeID);
        modelStatement->executeUpdate();

        conn->commit();
        conn->setAutoCommit(true);

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

    if (conn)
    {
        try
        {
            conn->rollback();
            conn->setAutoCommit(true);
        }
        catch (...)
        {
        }
    }

    return false;
}

bool Database::AddVehicleType(int brandID, const std::string& name)
{
    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        if (name.empty())
            throw std::runtime_error("Vehicle type name is required.");

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "INSERT INTO vehicle_type (brand_id, name) "
                "VALUES (?, ?)"
            )
        );

        statement->setInt(1, brandID);
        statement->setString(2, name);

        statement->executeUpdate();

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

bool Database::AddModel(
    int vehicleTypeID,
    const std::string& name,
    int powerPS)
{
    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        if (powerPS < 0)
            throw std::runtime_error("Horsepower cannot be negative.");

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "INSERT INTO model (vehicle_type_id, name, power_ps) "
                "VALUES (?, NULLIF(?, ''), ?)"
            )
        );

        statement->setInt(1, vehicleTypeID);
        statement->setString(2, name);
        statement->setInt(3, powerPS);

        statement->executeUpdate();

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

bool Database::DeleteVehicleType(int vehicleTypeID)
{
    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "DELETE FROM vehicle_type "
                "WHERE id = ? "
                "AND NOT EXISTS ("
                "    SELECT 1 FROM model WHERE vehicle_type_id = ?"
                ")"
            )
        );

        statement->setInt(1, vehicleTypeID);
        statement->setInt(2, vehicleTypeID);

        int affectedRows = statement->executeUpdate();

        if (affectedRows == 0)
        {
            lastError = "Vehicle type not found or it still has models.";
            return false;
        }

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

bool Database::DeleteModel(int modelID)
{
    try
    {
        lastError.clear();

        if (!conn)
            throw std::runtime_error("Database is not connected.");

        std::unique_ptr<sql::PreparedStatement> statement(
            conn->prepareStatement(
                "DELETE FROM model WHERE id = ?"
            )
        );

        statement->setInt(1, modelID);

        int affectedRows = statement->executeUpdate();

        if (affectedRows == 0)
        {
            lastError = "Model not found.";
            return false;
        }

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