#pragma once

#include "Entities.h"

#include <optional>
#include <string>

class ICountryRepository
{
public:
    virtual ~ICountryRepository() = default;

    virtual std::optional<Country> GetByID(int id) = 0;

    // Returns the id of the country and creates it when it is not there yet.
    // Returns 0 when it fails; GetLastError() then holds the reason.
    virtual int FindOrCreate(const std::string& name) = 0;

    virtual std::string GetLastError() const = 0;
};
