#pragma once

#include "Window.h"
#include "Database.h"

class EditTypeWindow;

class DetailedWindow : public Window {
private:
    EditTypeWindow& editWindow;
    int typeID = 0;
    Database& db;
    CarDetails details;
    bool showKw = false;               // false = show PK, true = show kW (the choice stays when you open another car)

    int PkToKw(int pk) const;          // converts PK to kW (1 PK = 0.7355 kW)

protected:
    void DrawContent() override;

public:
    DetailedWindow(Database& database, EditTypeWindow& editTypeWindow);

    void Show(int typeID);
    void Reload() override;
};