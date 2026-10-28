
#pragma once

#include <vector>
#include "Database.h"
#include "Window.h"
#include "TypeWindow.h"
#include "BrandWindow.h"
#include "DetailedWindow.h"
#include "EditTypeWindow.h"

struct GLFWwindow;

class App {
private:
    Database& db;

    GLFWwindow* window = nullptr;

    EditTypeWindow editTypeWindow;
    TypeWindow typeWindow;
    BrandWindow brandWindow;
    DetailedWindow detailedWindow;

    std::vector<Window*> windows;

public:
    App(Database& database);
    int Run();
};
