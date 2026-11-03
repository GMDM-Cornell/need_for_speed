#pragma once

#include "Window.h"
#include "Database.h"
#include "ImageManager.h"

class EditWindow;

class DetailedWindow : public Window
{
private:
    Database& db;
    ImageManager& imageManager;
    EditWindow& editWindow;

    int selectedBrandID = -1;
    int selectedTypeID = -1;
    int selectedModelID = -1;

    std::string brandName;

    BrandLocation location;
    std::vector<CarDetails> models;

    int showKW = 0;

    char newModelName[128] = {};
    int newModelPowerPS = 0;

    std::string errorMessage;

    void RefreshModels();

public:
    DetailedWindow(
        Database& database,
        ImageManager& imageManager,
        EditWindow& editWindow
    );

    void Show(int brandID, const std::string& brandName, int typeID);

    void DrawContent() override;
};