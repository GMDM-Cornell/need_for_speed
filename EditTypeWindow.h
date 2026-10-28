#pragma once
#include <string>
#include "EditWindow.h"

// Edit window for one car type: add a new type, change one or delete one.
class EditTypeWindow : public EditWindow {
private:
    bool isNew = true;                  // true = adding, false = changing an existing type
    TypeEdit original;                  // the values as they were when the window opened
    int brandID = 0;                    // the brand the type belongs to (not editable here)
    std::string brandName;
    char nameBuffer[256] = "";          // text the user types
    int pk = 0;

protected:
    void DrawFields() override;
    bool Save() override;
    bool Delete() override;
    bool CanDelete() const override;

public:
    EditTypeWindow(Database& database);

    void ShowNew(int forBrandID, const std::string& forBrandName);   // empty fields
    void ShowEdit(int forTypeID);                                    // fields filled from the database
};
