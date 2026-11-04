#pragma once

#include "IWindow.h"
#include "database.h"
#include "ImageManager.h"

#include "dal/BrandRepository.h"
#include "dal/CityRepository.h"
#include "dal/CountryRepository.h"
#include "dal/ModelRepository.h"
#include "dal/TypeRepository.h"

#include <memory>
#include <vector>

class App
{
private:
    // Repositories are declared before the windows, so they outlive them.
    CountryRepository countryRepository;
    CityRepository cityRepository;
    BrandRepository brandRepository;
    TypeRepository typeRepository;
    ModelRepository modelRepository;

    std::vector<std::unique_ptr<IWindow>> windows;

public:
    App(Database& database, ImageManager& imageManager);

    void AddWindow(std::unique_ptr<IWindow> window, bool openAtStart = false);
    void Render();
};
