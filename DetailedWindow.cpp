#include "DetailedWindow.h"
#include "imgui.h"
#include <cmath>
#include "EditTypeWindow.h"

DetailedWindow::DetailedWindow(Database& database, EditTypeWindow& editTypeWindow)
    : Window("Car Details"), db(database), editWindow(editTypeWindow)
{
    closable = true;
    open = false;
}

void DetailedWindow::Show(int typeID)
{
    this->typeID = typeID;
    details = db.GetCarDetails(typeID);   // one query per click, not per frame
    open = true;
}

int DetailedWindow::PkToKw(int pk) const
{
    return static_cast<int>(std::lround(pk * 0.7355));
}

void DetailedWindow::DrawContent()
{
    ImGui::Text("Car Details");
    ImGui::Separator();

    ImGui::Text("Land: %s", details.country.c_str());
    ImGui::Text("Stad: %s", details.city.c_str());
    ImGui::Text("Merk: %s", details.brand.c_str());
    ImGui::Text("Type: %s", details.type.c_str());

    // The power is shown in the unit that was chosen in the popup.
    if (showKw) {
        ImGui::Text("kW: %d", PkToKw(details.pk));   // converted from PK
    }
    else {
        ImGui::Text("PK: %d", details.pk);
    }

    ImGui::SameLine();
    if (ImGui::SmallButton("Eenheid")) {
        ImGui::OpenPopup("EenheidPopup");            // opens the popup on top of this window
    }

    if (ImGui::BeginPopup("EenheidPopup")) {         // only true while the popup is open
        if (ImGui::RadioButton("PK", !showKw)) showKw = false;
        if (ImGui::RadioButton("kW", showKw)) showKw = true;
        ImGui::EndPopup();                           // only called when BeginPopup returned true
    }

    ImGui::Separator();
    if (ImGui::Button("Bewerken")) {
        editWindow.ShowEdit(typeID);
    }
}

void DetailedWindow::Reload() {
    if (!open) return;
    details = db.GetCarDetails(typeID);
    if (details.type.empty()) {     // het type is verwijderd: sluit dit window
        open = false;
    }
}