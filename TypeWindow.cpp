#include "TypeWindow.h"
#include "imgui.h"
#include "DetailedWindow.h"
#include "EditTypeWindow.h"

TypeWindow::TypeWindow(
    Database& database,
    DetailedWindow& detailWin,
    EditTypeWindow& editTypeWindow
)
    : Window("Types"),
    db(database),
    detailedWindow(detailWin),
    editWindow(editTypeWindow)
{
    closable = true;
    open = false;
}

void TypeWindow::Show(int brandID, const std::string& name) {
    selectedBrandID = brandID;
    brandName = name;
    types = db.GetTypes(brandID);
    selectedTypeID = -1;
    open = true;
    location = db.GetBrandLocation(selectedBrandID);
}

void TypeWindow::DrawContent() {
    ImGui::Text("Types of %s:", brandName.c_str());

    ImGui::Text("Gevestigd in: %s, %s", location.city.c_str(), location.country.c_str());

    ImGui::Separator();

    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

    if (ImGui::BeginTable("TypesTabel", 2, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 40.0f);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        for (int i = 0; i < static_cast<int>(types.size()); i++) {
            ImGui::PushID(types[i].id);
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", i + 1);

            ImGui::TableSetColumnIndex(1);
            if (ImGui::Selectable(
                types[i].name.c_str(),
                selectedTypeID == types[i].id))
            {
                selectedTypeID = types[i].id;

                detailedWindow.Show(types[i].id);
            }

            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    ImGui::Separator();

    if (ImGui::Button("Nieuw type")) {
        editWindow.ShowNew(selectedBrandID, brandName);
    }
    ImGui::SameLine();

    ImGui::BeginDisabled(selectedTypeID == -1);     // niets geselecteerd = knop grijs
    if (ImGui::Button("Bewerken")) {
        editWindow.ShowEdit(selectedTypeID);
    }
    ImGui::EndDisabled();
}

void TypeWindow::Reload() {
    if (selectedBrandID == -1) return;
    types = db.GetTypes(selectedBrandID);
    selectedTypeID = -1;
}