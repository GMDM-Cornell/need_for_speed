#pragma once
#include <memory>
#include <string>
#include <vector>
#include <mariadb/conncpp.hpp>

struct CarDetails {
    std::string country;
    std::string city;
    std::string brand;
    std::string type;
    int pk = 0;
};

struct ListItem {
    int id;
    std::string name;
};

struct BrandLocation {
    std::string country;
    std::string city;
};

struct TypeEdit {
    int id = 0;                      
    int brandID = 0;
    std::string brandName;
    std::string name;
    int pk = 0;
};

class Database {
private:
    std::unique_ptr<sql::Connection> conn;
    std::string lastError;

public:
    bool Connect(const std::string& user, const std::string& password);
    const std::string& GetLastError() const;

    std::vector<ListItem> GetBrands();
    std::vector<ListItem> GetTypes(int brandID);
    BrandLocation GetBrandLocation(int brandID);
    CarDetails GetCarDetails(int typeID);

    TypeEdit GetTypeForEdit(int typeID);
    bool AddType(int brandID, const std::string& name, int pk);
    bool EditType(const TypeEdit& original, const std::string& newName, int newPk);
    bool DeleteType(const TypeEdit& original);
};