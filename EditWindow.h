#pragma once
#include <string>
#include "Window.h"
#include "Database.h"

// Abstract base class for all edit windows.
// It draws the Save / Cancel / Delete buttons, the delete confirmation and the error message.
// A derived class only supplies its own input fields and what saving / deleting does.
class EditWindow : public Window {
protected:
    Database& db;
    std::string errorMessage;           // shown in red under the fields
    bool dataChanged = false;           // true after a successful save or delete

    virtual void DrawFields() = 0;      // abstract: the input fields
    virtual bool Save() = 0;            // abstract: write to the database, true = success
    virtual bool Delete() = 0;          // abstract: delete from the database, true = success
    virtual bool CanDelete() const = 0; // false while a new item is being added

    void DrawContent() override;        // fields + buttons, the same for every edit window

public:
    EditWindow(Database& database, const std::string& windowTitle);

    // Returns true once after a change, so App can reload the other windows.
    bool TakeChanged();
};