#include "TypeRepository.h"

#include "../database.h"

#include <mariadb/conncpp.hpp>

#include <memory>
#include <stdexcept>

TypeRepository::TypeRepository(Database& database)
    : RepositoryBase(database)
{
}

std::string TypeRepository::GetLastError() const
{
    return RepositoryBase::GetLastError();
}

std::vector<VehicleType> TypeRepository::GetByBrand(int brandID)
{
    std::vector<VehicleType> types;

    Run([&]
    {
        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement(
                "SELECT id, brand_id, name FROM vehicle_type "
                "WHERE brand_id = ? ORDER BY name"
            )
        );

        statement->setInt(1, brandID);

        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

        while (result->next())
        {
            VehicleType type;

            type.id = result->getInt("id");
            type.brandID = result->getInt("brand_id");
            type.name = result->getString("name");

            types.push_back(type);
        }
    });

    return types;
}

bool TypeRepository::Add(int brandID, const std::string& name)
{
    return Run([&]
    {
        if (name.empty())
        {
            throw std::runtime_error("Vehicle type name is required.");
        }

        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement(
                "INSERT INTO vehicle_type (brand_id, name) VALUES (?, ?)"
            )
        );

        statement->setInt(1, brandID);
        statement->setString(2, name);
        statement->executeUpdate();
    });
}

bool TypeRepository::Rename(int vehicleTypeID, const std::string& name)
{
    return Run([&]
    {
        if (name.empty())
        {
            throw std::runtime_error("Vehicle type name is required.");
        }

        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement(
                "UPDATE vehicle_type SET name = ? WHERE id = ?"
            )
        );

        statement->setString(1, name);
        statement->setInt(2, vehicleTypeID);

        // Renaming to the same name changes no row; that is not an error.
        if (statement->executeUpdate() == 0 && !Exists("vehicle_type", vehicleTypeID))
        {
            throw std::runtime_error("Vehicle type not found.");
        }
    });
}

bool TypeRepository::Remove(int vehicleTypeID)
{
    return Run([&]
    {
        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement(
                "DELETE FROM vehicle_type WHERE id = ? "
                "AND NOT EXISTS (SELECT 1 FROM model WHERE vehicle_type_id = ?)"
            )
        );

        statement->setInt(1, vehicleTypeID);
        statement->setInt(2, vehicleTypeID);

        if (statement->executeUpdate() == 0)
        {
            throw std::runtime_error("Vehicle type not found or it still has models.");
        }
    });
}
