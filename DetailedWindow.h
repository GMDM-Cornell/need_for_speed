#pragma once

#include "Window.h"
#include "dal/ModelRepository.h"
#include "ImageManager.h"

class EditWindow;

class DetailedWindow : public Window
{
private:
    ModelRepository& modelRepository;
    ImageManager& imageManager;
    EditWindow& editWindow;

    Brand brand;
    City city;
    Country country;

    int selectedTypeID = -1;
    int selectedModelID = -1;

    std::string typeName;

    std::vector<Model> models;

    int showKW = 0;

    char newModelName[128] = {};
    int newModelPowerPS = 0;

    std::string errorMessage;

    void RefreshModels();

public:
    DetailedWindow(
        ModelRepository& modelRepository,
        ImageManager& imageManager,
        EditWindow& editWindow
    );

    void Show(
        const Brand& brand,
        const City& city,
        const Country& country,
        const VehicleType& type
    );

    void DrawContent() override;
};
