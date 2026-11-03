#pragma once

#include "Window.h"
#include "Database.h"
#include "ImageManager.h"

class TypeWindow;

class BrandWindow : public Window
{
private:
    Database& db;
    ImageManager& imageManager;
    TypeWindow& typeWindow;

    std::vector<Brand> brands;

    int selectedBrandID = -1;

    void RefreshBrands();

public:
    BrandWindow(
        Database& database,
        ImageManager& imageManager,
        TypeWindow& typeWindow
    );

    void DrawContent() override;
};