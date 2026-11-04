#pragma once

#include "Window.h"
#include "dal/IBrandRepository.h"
#include "dal/IModelRepository.h"

#include <functional>

class EditWindow : public Window
{
private:
    IBrandRepository& brandRepository;
    IModelRepository& modelRepository;

    int brandID = -1;
    int modelID = -1;

    char countryName[128] = {};
    char cityName[128] = {};
    char modelName[128] = {};

    int powerPS = 0;

    std::string saveError;

    std::function<void()> onSaved;

public:
    EditWindow(
        IBrandRepository& brandRepository,
        IModelRepository& modelRepository
    );

    void Show(
        const Brand& brand,
        const City& city,
        const Country& country,
        const Model& model,
        std::function<void()> callback
    );

    void DrawContent() override;
};
