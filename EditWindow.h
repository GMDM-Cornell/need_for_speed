#pragma once

#include "Window.h"
#include "Database.h"

#include <functional>

class EditWindow : public Window
{
private:
    Database& db;

    int brandID = -1;
    int modelID = -1;
    int vehicleTypeID = -1;

    char country[128] = {};
    char city[128] = {};
    char modelName[128] = {};

    int powerPS = 0;

    std::string saveError;

    std::function<void()> onSaved;

public:
    EditWindow(Database& database);

    void Show(
        int selectedBrandID,
        const BrandLocation& location,
        const CarDetails& details,
        std::function<void()> callback
    );

    void DrawContent() override;
};