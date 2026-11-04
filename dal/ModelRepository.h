#pragma once

#include "IModelRepository.h"
#include "RepositoryBase.h"

class Database;

class ModelRepository : public IModelRepository, private RepositoryBase
{
public:
    explicit ModelRepository(Database& database);

    std::vector<Model> GetByType(int vehicleTypeID) override;
    bool Add(int vehicleTypeID, const std::string& name, int powerPS) override;
    bool Update(int modelID, const std::string& name, int powerPS) override;
    bool Remove(int modelID) override;

    std::string GetLastError() const override;
};
