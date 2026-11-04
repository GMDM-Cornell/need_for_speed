#pragma once

#include "Entities.h"

#include <string>
#include <vector>

class IModelRepository
{
public:
    virtual ~IModelRepository() = default;

    virtual std::vector<Model> GetByType(int vehicleTypeID) = 0;
    virtual bool Add(int vehicleTypeID, const std::string& name, int powerPS) = 0;
    virtual bool Update(int modelID, const std::string& name, int powerPS) = 0;
    virtual bool Remove(int modelID) = 0;

    virtual std::string GetLastError() const = 0;
};
