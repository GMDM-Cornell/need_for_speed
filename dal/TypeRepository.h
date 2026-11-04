#pragma once

#include "ITypeRepository.h"
#include "RepositoryBase.h"

class Database;

class TypeRepository : public ITypeRepository, private RepositoryBase
{
public:
    explicit TypeRepository(Database& database);

    std::vector<VehicleType> GetByBrand(int brandID) override;
    bool Add(int brandID, const std::string& name) override;
    bool Rename(int vehicleTypeID, const std::string& name) override;
    bool Remove(int vehicleTypeID) override;

    std::string GetLastError() const override;
};
