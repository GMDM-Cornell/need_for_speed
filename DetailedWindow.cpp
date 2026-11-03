#include "DetailedWindow.h"
#include "EditWindow.h"
#include "imgui.h"

#include <cstdint>
#include <cmath>

DetailedWindow::DetailedWindow(
    Database& database,
    ImageManager& imageManager,
    EditWindow& editWindow)
    : Window("Car Details"),
    db(database),
    imageManager(imageManager),
    editWindow(editWindow)
{
}

void DetailedWindow::Show(int brandID, const std::string& name, int typeID)
{
    selectedBrandID = brandID;
    selectedTypeID = typeID;
    selectedModelID = -1;

    brandName = name;
    showKW = 0;
    errorMessage.clear();

    location = db.GetBrandLocation(brandID);

    RefreshModels();

    Open();
}

void DetailedWindow::RefreshModels()
{
    models = db.GetCarDetails(selectedTypeID);

    selectedModelID = -1;

    if (models.size() == 1)
    {
        selectedModelID = models[0].modelID;
    }
}

void DetailedWindow::DrawContent()
{
    ImGui::Text("Brand: %s", brandName.c_str());

    ImGui::Text(
        "Headquarters: %s, %s",
        location.city.c_str(),
        location.country.c_str()
    );

    ImGui::Separator();

    ImGui::Text("Add a model");

    ImGui::InputText(
        "Model name",
        newModelName,
        sizeof(newModelName)
    );

    ImGui::InputInt("Power (PS)", &newModelPowerPS);

    if (ImGui::Button("Add model"))
    {
        if (db.AddModel(selectedTypeID, newModelName, newModelPowerPS))
        {
            errorMessage.clear();

            RefreshModels();

            for (const CarDetails& model : models)
            {
                if (model.modelName == newModelName &&
                    model.powerPS == newModelPowerPS)
                {
                    selectedModelID = model.modelID;
                }
            }

            newModelName[0] = '\0';
            newModelPowerPS = 0;
        }
        else
        {
            errorMessage = db.GetLastError();
        }
    }

    ImGui::Separator();

    if (models.empty())
    {
        ImGui::Text("No models found.");
    }
    else if (models.size() > 1)
    {
        ImGui::Text("Available models");

        if (ImGui::BeginTable(
            "ModelsTable",
            1,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            ImGui::TableSetupColumn("Model");
            ImGui::TableHeadersRow();

            for (const CarDetails& model : models)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                ImGui::PushID(model.modelID);

                const char* modelName = model.modelName.empty()
                    ? "Unspecified model"
                    : model.modelName.c_str();

                if (ImGui::Selectable(
                    modelName,
                    selectedModelID == model.modelID))
                {
                    selectedModelID = model.modelID;
                }

                ImGui::PopID();
            }

            ImGui::EndTable();
        }
    }

    const CarDetails* selectedModel = nullptr;

    for (const CarDetails& model : models)
    {
        if (model.modelID == selectedModelID)
        {
            selectedModel = &model;
            break;
        }
    }

    if (selectedModel != nullptr)
    {
        ImGui::Separator();

        unsigned int texture = imageManager.GetTexture(selectedBrandID);

        if (texture != 0)
        {
            ImGui::Image(
                (ImTextureID)(intptr_t)texture,
                ImVec2(150.0f, 100.0f)
            );
        }

        ImGui::Text(
            "Vehicle type: %s",
            selectedModel->vehicleTypeName.c_str()
        );

        if (!selectedModel->modelName.empty())
        {
            ImGui::Text("Model: %s", selectedModel->modelName.c_str());
        }
        else
        {
            ImGui::Text("Model: Unspecified");
        }

        ImGui::Separator();

        ImGui::RadioButton("PS", &showKW, 0);
        ImGui::SameLine();
        ImGui::RadioButton("kW", &showKW, 1);

        ImGui::Separator();

        if (showKW == 1)
        {
            int powerKW = static_cast<int>(
                std::round(selectedModel->powerPS * 0.7355f)
                );

            ImGui::Text("Power: %d kW", powerKW);
        }
        else
        {
            ImGui::Text("Power: %d PS", selectedModel->powerPS);
        }

        ImGui::Separator();

        if (ImGui::Button("Edit model"))
        {
            editWindow.Show(
                selectedBrandID,
                location,
                *selectedModel,
                [this]()
                {
                    RefreshModels();
                }
            );
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete selected model"))
        {
            if (db.DeleteModel(selectedModelID))
            {
                errorMessage.clear();
                RefreshModels();
            }
            else
            {
                errorMessage = db.GetLastError();
            }
        }
    }
    else if (!models.empty())
    {
        ImGui::Text("Select a model to see its details.");
    }

    if (!errorMessage.empty())
    {
        ImGui::Separator();
        ImGui::TextWrapped("Error: %s", errorMessage.c_str());
    }
}