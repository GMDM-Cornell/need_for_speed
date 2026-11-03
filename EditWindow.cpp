#include "EditWindow.h"
#include "imgui.h"

#include <cstdio>

EditWindow::EditWindow(Database& database)
    : Window("Edit Car Details"),
    db(database)
{
}

void EditWindow::Show(
    int selectedBrandID,
    const BrandLocation& selectedLocation,
    const CarDetails& details,
    std::function<void()> callback)
{
    saveError.clear();

    brandID = selectedBrandID;
    modelID = details.modelID;
    vehicleTypeID = details.vehicleTypeID;

    std::snprintf(
        country,
        sizeof(country),
        "%s",
        selectedLocation.country.c_str()
    );

    std::snprintf(
        city,
        sizeof(city),
        "%s",
        selectedLocation.city.c_str()
    );

    std::snprintf(
        modelName,
        sizeof(modelName),
        "%s",
        details.modelName.c_str()
    );

    powerPS = details.powerPS;

    onSaved = callback;

    Open();
}

void EditWindow::DrawContent()
{
    ImGui::Text("Edit headquarters");

    ImGui::InputText("Country", country, sizeof(country));
    ImGui::InputText("City", city, sizeof(city));

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
        else if (db.UpdateBrandLocation(brandID, country, city))
        {
            if (db.UpdateModel(modelID, modelName, powerPS))
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
                saveError = db.GetLastError();
            }
        }
        else
        {
            saveError = db.GetLastError();
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