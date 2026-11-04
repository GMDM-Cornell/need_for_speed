#pragma once

#include "Entities.h"

#include <string>
#include <vector>

class ITypeRepository
{
public:
    virtual ~ITypeRepository() = default;

    virtual std::vector<VehicleType> GetByBrand(int brandID) = 0;
    virtual bool Add(int brandID, const std::string& name) = 0;
    virtual bool Rename(int vehicleTypeID, const std::string& name) = 0;
    virtual bool Remove(int vehicleTypeID) = 0;

    virtual std::string GetLastError() const = 0;
};
