#include "BrandWindow.h"
#include "TypeWindow.h"
#include "imgui.h"

#include <cstdint>

BrandWindow::BrandWindow(
    BrandRepository& brandRepository,
    ImageManager& imageManager,
    TypeWindow& typeWindow)
    : Window("Brands", false),
    brandRepository(brandRepository),
    imageManager(imageManager),
    typeWindow(typeWindow)
{
    RefreshBrands();
}

void BrandWindow::RefreshBrands()
{
    imageManager.Clear();

    brands = brandRepository.GetAll();

    for (const Brand& brand : brands)
    {
        imageManager.LoadLogo(brand.id, brand.logo);
    }
}

void BrandWindow::DrawContent()
{
    ImGui::Text("Available brands");

    if (brands.empty())
    {
        const std::string error = brandRepository.GetLastError();

        if (!error.empty())
        {
            ImGui::TextWrapped("Error: %s", error.c_str());
        }
    }

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
                typeWindow.Show(brand.id);
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}
