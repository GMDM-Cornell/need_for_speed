#pragma once

#include "Entities.h"

#include <optional>
#include <string>

class ICityRepository
{
public:
    virtual ~ICityRepository() = default;

    virtual std::optional<City> GetByID(int id) = 0;

    // Returns the id of the city inside that country, creating it when needed.
    // Returns 0 when it fails; GetLastError() then holds the reason.
    virtual int FindOrCreate(int countryID, const std::string& name) = 0;

    virtual std::string GetLastError() const = 0;
};
