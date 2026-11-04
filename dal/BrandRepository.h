#pragma once

#include "IBrandRepository.h"
#include "RepositoryBase.h"

class Database;
class ICityRepository;
class ICountryRepository;

class BrandRepository : public IBrandRepository, private RepositoryBase
{
public:
    BrandRepository(
        Database& database,
        ICityRepository& cityRepository,
        ICountryRepository& countryRepository
    );

    std::vector<Brand> GetAll() override;
    std::optional<Brand> GetByID(int brandID) override;

    bool SetHeadquarters(
        int brandID,
        const std::string& city,
        const std::string& country
    ) override;

    std::string GetLastError() const override;

private:
    ICityRepository& cityRepository;
    ICountryRepository& countryRepository;
};
