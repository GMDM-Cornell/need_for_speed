#pragma once

#include "Entities.h"
#include "RepositoryBase.h"

#include <optional>

class Database;

class CountryRepository : private RepositoryBase
{
public:
    explicit CountryRepository(Database& database);

    std::optional<Country> GetByID(int id);
    int FindOrCreate(const std::string& name);
    std::string GetLastError() const;
};
