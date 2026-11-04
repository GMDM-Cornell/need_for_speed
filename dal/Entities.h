#pragma once

#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

// One struct per table: the members are the columns of that table, nothing else.
// The application works with these objects; it never sees a row, a column name or SQL.

struct Country
{
    int id = 0;
    std::string name;
};

struct City
{
    int id = 0;
    int countryID = 0;
    std::string name;
};

struct Brand
{
    int id = 0;
    int cityID = 0;
    std::string name;
    std::vector<std::uint8_t> logo;
};

struct VehicleType
{
    int id = 0;
    int brandID = 0;
    std::string name;
};

struct Model
{
    int id = 0;
    int vehicleTypeID = 0;
    std::string name;
    int powerPS = 0;

    // Same rule as ps_to_kw() in stored_nfs.sql, so the GUI and the view show the same kW.
    int PowerKW() const
    {
        return static_cast<int>(std::lround(powerPS * 0.73549875));
    }
};
