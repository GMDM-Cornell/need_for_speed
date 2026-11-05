#pragma once

#include "Entities.h"
#include "RepositoryBase.h"

#include <optional>

class Database;

class CityRepository : private RepositoryBase
{
public:
    explicit CityRepository(Database& database);

    std::optional<City> GetByID(int id);
    int FindOrCreate(int countryID, const std::string& name);
    std::string GetLastError() const;
};
