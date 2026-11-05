#pragma once

#include "Entities.h"
#include "RepositoryBase.h"

class Database;

class ModelRepository : private RepositoryBase
{
public:
    explicit ModelRepository(Database& database);

    std::vector<Model> GetByType(int vehicleTypeID);
    bool Add(int vehicleTypeID, const std::string& name, int powerPS);
    bool Update(int modelID, const std::string& name, int powerPS);
    bool Remove(int modelID);

    std::string GetLastError() const;
};
