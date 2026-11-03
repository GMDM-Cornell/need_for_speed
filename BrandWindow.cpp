#include "BrandWindow.h"
#include "TypeWindow.h"
#include "imgui.h"

#include <cstdint>

BrandWindow::BrandWindow(
    Database& database,
    ImageManager& imageManager,
    TypeWindow& typeWindow)
    : Window("Brands", false),
    db(database),
    imageManager(imageManager),
    typeWindow(typeWindow)
{
    RefreshBrands();
}

void BrandWindow::RefreshBrands()
{
    imageManager.Clear();

    brands = db.GetBrands();

    for (const Brand& brand : brands)
    {
        std::vector<std::uint8_t> logo = db.GetBrandLogo(brand.id);

        if (!logo.empty())
        {
            imageManager.LoadLogo(brand.id, logo);
        }
    }
}

void BrandWindow::DrawContent()
{
    ImGui::Text("Available brands");

    if (ImGui::BeginTable(
        "BrandTable",
        2,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Logo");
        ImGui::TableSetupColumn("Brand");

        ImGui::TableHeadersRow();

        for (const Brand& brand : brands)
        {
            ImGui::TableNextRow(0, 72.0f);

            ImGui::TableSetColumnIndex(0);

            unsigned int texture = imageManager.GetTexture(brand.id);

            if (texture != 0)
            {
                ImGui::Image(
                    (ImTextureID)(intptr_t)texture,
                    ImVec2(64.0f, 64.0f)
                );
            }
            else
            {
                ImGui::Text("No logo");
            }

            ImGui::TableSetColumnIndex(1);

            ImGui::PushID(brand.id);

            if (ImGui::Selectable(
                brand.name.c_str(),
                selectedBrandID == brand.id))
            {
                selectedBrandID = brand.id;
                typeWindow.Show(brand.id, brand.name);
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}