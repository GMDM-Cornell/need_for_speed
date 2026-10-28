#pragma once
#include <vector>
#include "Window.h"
#include "Database.h"

class TypeWindow;

class BrandWindow : public Window {
private:
    Database& db;
    TypeWindow& typeWindow;
    std::vector<ListItem> brands;
    int selectedBrandID = -1;

protected:
    void DrawContent() override;

public:
    void Reload() override;
    BrandWindow(Database& database, TypeWindow& typeWin);
};