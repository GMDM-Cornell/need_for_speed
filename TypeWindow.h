#pragma once
#include <string>
#include <vector>
#include "Window.h"
#include "Database.h"

class DetailedWindow;
class EditTypeWindow;

class TypeWindow : public Window {
private:
    DetailedWindow& detailedWindow;
    EditTypeWindow& editWindow;
    Database& db;
    std::string brandName;
    std::vector<ListItem> types;
    BrandLocation location;
    int selectedTypeID = -1;
    int selectedBrandID = -1;
protected:
    void DrawContent() override;

public:
    TypeWindow(Database& database, DetailedWindow& detailWin, EditTypeWindow& editTypeWindow);
    void Show(int brandID, const std::string& name);
    void Reload() override;
};