#pragma once

#include "Entities.h"

#include <optional>
#include <string>
#include <vector>

class IBrandRepository
{
public:
    virtual ~IBrandRepository() = default;

    // Every brand with its logo, because that is the row the table holds.
    virtual std::vector<Brand> GetAll() = 0;

    // One brand, read again from the table so a changed headquarters shows up.
    virtual std::optional<Brand> GetByID(int brandID) = 0;

    // Moves a brand to another city, creating country and city when they are new.
    virtual bool SetHeadquarters(
        int brandID,
        const std::string& city,
        const std::string& country
    ) = 0;

    virtual std::string GetLastError() const = 0;
};
