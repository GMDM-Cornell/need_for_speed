#include "EditWindow.h"
#include "imgui.h"

#include <cstdio>

EditWindow::EditWindow(
    BrandRepository& brandRepository,
    ModelRepository& modelRepository)
    : Window("Edit Car Details"),
    brandRepository(brandRepository),
    modelRepository(modelRepository)
{
}

void EditWindow::Show(
    const Brand& brand,
    const City& city,
    const Country& country,
    const Model& model,
    std::function<void()> callback)
{
    saveError.clear();

    brandID = brand.id;
    modelID = model.id;

    std::snprintf(
        countryName,
        sizeof(countryName),
        "%s",
        country.name.c_str()
    );

    std::snprintf(
        cityName,
        sizeof(cityName),
        "%s",
        city.name.c_str()
    );

    std::snprintf(
        modelName,
        sizeof(modelName),
        "%s",
        model.name.c_str()
    );

    powerPS = model.powerPS;

    onSaved = callback;

    Open();
}

void EditWindow::DrawContent()
{
    ImGui::Text("Edit headquarters");

    ImGui::InputText("Country", countryName, sizeof(countryName));
    ImGui::InputText("City", cityName, sizeof(cityName));

    ImGui::Separator();

    ImGui::Text("Edit model");

    ImGui::InputText("Model", modelName, sizeof(modelName));
    ImGui::InputInt("Power (PS)", &powerPS);

    ImGui::Separator();

    if (ImGui::Button("Save"))
    {
        if (powerPS < 0)
        {
            saveError = "Horsepower cannot be negative.";
        }
        else if (brandRepository.SetHeadquarters(brandID, cityName, countryName))
        {
            if (modelRepository.Update(modelID, modelName, powerPS))
            {
                saveError.clear();

                if (onSaved)
                {
                    onSaved();
                }

                Close();
            }
            else
            {
                saveError = modelRepository.GetLastError();
            }
        }
        else
        {
            saveError = brandRepository.GetLastError();
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel"))
    {
        Close();
    }

    if (!saveError.empty())
    {
        ImGui::Separator();
        ImGui::TextWrapped("Error: %s", saveError.c_str());
    }
}
