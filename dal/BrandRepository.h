#pragma once

#include "Entities.h"
#include "RepositoryBase.h"

#include <optional>

class Database;
class CityRepository;
class CountryRepository;

class BrandRepository : private RepositoryBase
{
public:
    BrandRepository(
        Database& database,
        CityRepository& cityRepository,
        CountryRepository& countryRepository
    );

    std::vector<Brand> GetAll();
    std::optional<Brand> GetByID(int brandID);

    bool SetHeadquarters(
        int brandID,
        const std::string& city,
        const std::string& country
    );

    std::string GetLastError() const;

private:
    CityRepository& cityRepository;
    CountryRepository& countryRepository;
};
