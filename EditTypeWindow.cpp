#include "EditTypeWindow.h"
#include "imgui.h"
#include <cstdio>

namespace {
    const int MinPk = 1;
    const int MaxPk = 5000;             // sanity limit, change it if you need to

    // Removes spaces, tabs and line breaks at the start and the end
    std::string Trim(const std::string& text) {
        const char* whitespace = " \t\r\n";
        size_t first = text.find_first_not_of(whitespace);
        if (first == std::string::npos) return "";
        size_t last = text.find_last_not_of(whitespace);
        return text.substr(first, last - first + 1);
    }
}

EditTypeWindow::EditTypeWindow(Database& database)
    : EditWindow(database, "Type bewerken") {}

void EditTypeWindow::ShowNew(int forBrandID, const std::string& forBrandName) {
    isNew = true;
    original = TypeEdit();
    brandID = forBrandID;
    brandName = forBrandName;
    nameBuffer[0] = '\0';
    pk = 0;
    errorMessage.clear();
    open = true;
}

void EditTypeWindow::ShowEdit(int forTypeID) {
    isNew = false;
    errorMessage.clear();

    original = db.GetTypeForEdit(forTypeID);         // one query, when the window opens
    if (original.id == 0) {                          // not found, for example already deleted
        errorMessage = db.GetLastError();
        open = true;
        return;
    }

    brandID = original.brandID;
    brandName = original.brandName;
    pk = original.pk;
    std::snprintf(nameBuffer, sizeof(nameBuffer), "%s", original.name.c_str());
    open = true;
}

void EditTypeWindow::DrawFields() {
    if (isNew) {
        ImGui::Text("Nieuw type voor merk: %s", brandName.c_str());
    }
    else {
        ImGui::Text("Merk: %s", brandName.c_str());
    }

    ImGui::InputText("Naam", nameBuffer, sizeof(nameBuffer));
    ImGui::InputInt("PK", &pk);
}

bool EditTypeWindow::Save() {
    if (!isNew && original.id == 0) {
        errorMessage = "Er is niets om op te slaan.";
        return false;
    }

    // No empty data: check everything before it goes to the database.
    // The database checks it again as a second line of defence.
    std::string name = Trim(nameBuffer);
    if (name.empty()) {
        errorMessage = "De naam mag niet leeg zijn.";
        return false;
    }
    if (pk < MinPk || pk > MaxPk) {
        errorMessage = "De PK moet tussen " + std::to_string(MinPk) + " en " + std::to_string(MaxPk) + " liggen.";
        return false;
    }

    if (isNew) {
        if (!db.AddType(brandID, name, pk)) {
            errorMessage = db.GetLastError();
            return false;
        }
        return true;
    }

    if (name == original.name && pk == original.pk) {
        return true;                                 // nothing was changed, nothing to write
    }

    if (!db.EditType(original, name, pk)) {
        errorMessage = db.GetLastError();
        return false;
    }
    return true;
}

bool EditTypeWindow::Delete() {
    if (!db.DeleteType(original)) {
        errorMessage = db.GetLastError();
        return false;
    }
    return true;
}

bool EditTypeWindow::CanDelete() const {
    return !isNew && original.id != 0;
}
