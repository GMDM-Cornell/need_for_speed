#pragma once

#include "IWindow.h"
#include "Database.h"
#include "ImageManager.h"

#include <memory>
#include <vector>

class App
{
private:
    std::vector<std::unique_ptr<IWindow>> windows;

public:
    App(Database& database, ImageManager& imageManager);

    void AddWindow(std::unique_ptr<IWindow> window, bool openAtStart = false);
    void Render();
};