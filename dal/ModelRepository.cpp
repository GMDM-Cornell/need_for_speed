#include "ModelRepository.h"

#include "../database.h"

#include <mariadb/conncpp.hpp>

#include <memory>
#include <stdexcept>

ModelRepository::ModelRepository(Database& database)
    : RepositoryBase(database)
{
}

std::string ModelRepository::GetLastError() const
{
    return RepositoryBase::GetLastError();
}

std::vector<Model> ModelRepository::GetByType(int vehicleTypeID)
{
    std::vector<Model> models;

    Run([&]
    {
        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement(
                "SELECT id, vehicle_type_id, name, power_ps FROM model "
                "WHERE vehicle_type_id = ? ORDER BY name"
            )
        );

        statement->setInt(1, vehicleTypeID);

        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

        while (result->next())
        {
            Model model;

            model.id = result->getInt("id");
            model.vehicleTypeID = result->getInt("vehicle_type_id");
            model.name = result->getString("name");
            model.powerPS = result->getInt("power_ps");

            models.push_back(model);
        }
    });

    return models;
}

bool ModelRepository::Add(int vehicleTypeID, const std::string& name, int powerPS)
{
    return Run([&]
    {
        if (powerPS < 0)
        {
            throw std::runtime_error("Horsepower cannot be negative.");
        }

        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement(
                "INSERT INTO model (vehicle_type_id, name, power_ps) "
                "VALUES (?, NULLIF(?, ''), ?)"
            )
        );

        statement->setInt(1, vehicleTypeID);
        statement->setString(2, name);
        statement->setInt(3, powerPS);
        statement->executeUpdate();
    });
}

bool ModelRepository::Update(int modelID, const std::string& name, int powerPS)
{
    return Run([&]
    {
        if (powerPS < 0)
        {
            throw std::runtime_error("Horsepower cannot be negative.");
        }

        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement(
                "UPDATE model SET name = NULLIF(?, ''), power_ps = ? WHERE id = ?"
            )
        );

        statement->setString(1, name);
        statement->setInt(2, powerPS);
        statement->setInt(3, modelID);

        // Saving a model without changing anything changes no row; that is not an error.
        if (statement->executeUpdate() == 0 && !Exists("model", modelID))
        {
            throw std::runtime_error("Model not found.");
        }
    });
}

bool ModelRepository::Remove(int modelID)
{
    return Run([&]
    {
        std::unique_ptr<sql::PreparedStatement> statement(
            Connection().prepareStatement("DELETE FROM model WHERE id = ?")
        );

        statement->setInt(1, modelID);

        if (statement->executeUpdate() == 0)
        {
            throw std::runtime_error("Model not found.");
        }
    });
}
