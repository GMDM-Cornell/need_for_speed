#include "App.h"

#include "BrandWindow.h"
#include "TypeWindow.h"
#include "DetailedWindow.h"
#include "EditWindow.h"

#include <utility>

App::App(Database& database, ImageManager& imageManager)
    : countryRepository(database),
      cityRepository(database),
      brandRepository(database, cityRepository, countryRepository),
      typeRepository(database),
      modelRepository(database)
{
    auto editWindow = std::make_unique<EditWindow>(
        brandRepository,
        modelRepository
    );

    auto detailedWindow = std::make_unique<DetailedWindow>(
        modelRepository,
        imageManager,
        *editWindow
    );

    auto typeWindow = std::make_unique<TypeWindow>(
        typeRepository,
        cityRepository,
        countryRepository,
        *detailedWindow
    );

    auto brandWindow = std::make_unique<BrandWindow>(
        brandRepository,
        imageManager,
        *typeWindow
    );

    AddWindow(std::move(editWindow)); //Move ownership to App
    AddWindow(std::move(detailedWindow));
    AddWindow(std::move(typeWindow));
    AddWindow(std::move(brandWindow), true);
}

void App::AddWindow(std::unique_ptr<IWindow> window, bool openAtStart)
{
    if (openAtStart)
    {
        window->Open();
    }

    windows.push_back(std::move(window));
}

void App::Render()
{
    for (const auto& window : windows) //Auto makes C++ choose the data type
    {
        window->Render();
    }
}
