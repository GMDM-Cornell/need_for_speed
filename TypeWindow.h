#pragma once

#include "Window.h"
#include "Database.h"

class DetailedWindow;

class TypeWindow : public Window
{
private:
    Database& db;
    DetailedWindow& detailedWindow;

    int selectedBrandID = -1;
    int selectedTypeID = -1;

    std::string brandName;

    BrandLocation location;
    std::vector<VehicleType> types;

    char newTypeName[128] = {};
    char editedTypeName[128] = {};

    std::string errorMessage;

    void RefreshTypes();

public:
    TypeWindow(Database& database, DetailedWindow& detailedWindow);

    void Show(int brandID, const std::string& name);

    void DrawContent() override;
};