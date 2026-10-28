#include "BrandWindow.h"
#include "TypeWindow.h"
#include "imgui.h"

BrandWindow::BrandWindow(Database& database, TypeWindow& typeWin)
    : Window("Merken"), db(database), typeWindow(typeWin) {
    brands = db.GetBrands();
}

void BrandWindow::DrawContent() {
    ImGui::Text("Merken uit de database:");
    ImGui::Separator();

    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

    if (ImGui::BeginTable("MerkenTabel", 2, flags)) { 
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 40.0f);
        ImGui::TableSetupColumn("Merk", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        for (int i = 0; i < static_cast<int>(brands.size()); i++) {
            ImGui::PushID(brands[i].id);
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", i + 1);

            ImGui::TableSetColumnIndex(1);
            if (ImGui::Selectable(brands[i].name.c_str(), selectedBrandID == brands[i].id)) {
                selectedBrandID = brands[i].id;
                typeWindow.Show(brands[i].id, brands[i].name);
            }

            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}

void BrandWindow::Reload() {
    brands = db.GetBrands();
}