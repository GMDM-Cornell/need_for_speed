#pragma once

#include "ICountryRepository.h"
#include "RepositoryBase.h"

class Database;

class CountryRepository : public ICountryRepository, private RepositoryBase
{
public:
    explicit CountryRepository(Database& database);

    std::optional<Country> GetByID(int id) override;
    int FindOrCreate(const std::string& name) override;
    std::string GetLastError() const override;
};
