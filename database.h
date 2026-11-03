#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace sql
{
    class Connection;
}

struct Brand
{
    int id;
    std::string name;
};

struct VehicleType
{
    int id;
    int brandID;
    std::string name;
};

struct BrandLocation
{
    int countryID;
    std::string country;
    int cityID;
    std::string city;
};

struct CarDetails
{
    int modelID;
    int vehicleTypeID;
    std::string vehicleTypeName;
    std::string modelName;
    int powerPS;
};

class Database
{
private:
    std::unique_ptr<sql::Connection> conn;
    std::string lastError;

    int GetOrCreateCountryID(const std::string& country);
    int GetOrCreateCityID(const std::string& city, int countryID);

public:
    Database();
    ~Database();

    bool Connect(const std::string& user, const std::string& password);

    std::string GetLastError() const;

    std::vector<std::uint8_t> GetBrandLogo(int brandID);
    std::vector<Brand> GetBrands();
    std::vector<VehicleType> GetTypes(int brandID);
    std::vector<CarDetails> GetCarDetails(int typeID);

    BrandLocation GetBrandLocation(int brandID);

    bool UpdateBrandLocation(
        int brandID,
        const std::string& country,
        const std::string& city
    );

    bool UpdateVehicleType(int vehicleTypeID, const std::string& name);

    bool UpdateModel(int modelID, const std::string& name, int powerPS);

    bool AddVehicleType(int brandID, const std::string& name);

    bool AddModel(int vehicleTypeID, const std::string& name, int powerPS);

    bool DeleteVehicleType(int vehicleTypeID);

    bool DeleteModel(int modelID);

    bool UpdateCarType(const CarDetails& details);
};