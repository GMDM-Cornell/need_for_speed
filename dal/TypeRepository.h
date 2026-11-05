#pragma once

#include "Entities.h"
#include "RepositoryBase.h"

class Database;

class TypeRepository : private RepositoryBase
{
public:
    explicit TypeRepository(Database& database);

    std::vector<VehicleType> GetByBrand(int brandID);
    bool Add(int brandID, const std::string& name);
    bool Rename(int vehicleTypeID, const std::string& name);
    bool Remove(int vehicleTypeID);

    std::string GetLastError() const;
};
