#pragma once

#include "ICityRepository.h"
#include "RepositoryBase.h"

class Database;

class CityRepository : public ICityRepository, private RepositoryBase
{
public:
    explicit CityRepository(Database& database);

    std::optional<City> GetByID(int id) override;
    int FindOrCreate(int countryID, const std::string& name) override;
    std::string GetLastError() const override;
};
