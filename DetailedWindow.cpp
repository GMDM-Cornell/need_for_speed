#include "DetailedWindow.h"
#include "EditWindow.h"
#include "imgui.h"

#include <cstdint>

DetailedWindow::DetailedWindow(
    ModelRepository& modelRepository,
    ImageManager& imageManager,
    EditWindow& editWindow)
    : Window("Car Details"),
    modelRepository(modelRepository),
    imageManager(imageManager),
    editWindow(editWindow)
{
}

void DetailedWindow::Show(
    const Brand& selectedBrand,
    const City& selectedCity,
    const Country& selectedCountry,
    const VehicleType& type)
{
    brand = selectedBrand;
    city = selectedCity;
    country = selectedCountry;

    typeName = type.name;
    selectedTypeID = type.id;
    selectedModelID = -1;

    showKW = 0;
    errorMessage.clear();

    RefreshModels();

    Open();
}

void DetailedWindow::RefreshModels()
{
    models = modelRepository.GetByType(selectedTypeID);

    selectedModelID = -1;

    if (models.size() == 1)
    {
        selectedModelID = models[0].id;
    }
}

void DetailedWindow::DrawContent()
{
    ImGui::Text("Brand: %s", brand.name.c_str());

    ImGui::Text(
        "Headquarters: %s, %s",
        city.name.c_str(),
        country.name.c_str()
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
        if (modelRepository.Add(selectedTypeID, newModelName, newModelPowerPS))
        {
            errorMessage.clear();

            RefreshModels();

            for (const Model& model : models)
            {
                if (model.name == newModelName &&
                    model.powerPS == newModelPowerPS)
                {
                    selectedModelID = model.id;
                }
            }

            newModelName[0] = '\0';
            newModelPowerPS = 0;
        }
        else
        {
            errorMessage = modelRepository.GetLastError();
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

            for (const Model& model : models)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                ImGui::PushID(model.id);

                const char* modelName = model.name.empty()
                    ? "Unspecified model"
                    : model.name.c_str();

                if (ImGui::Selectable(
                    modelName,
                    selectedModelID == model.id))
                {
                    selectedModelID = model.id;
                }

                ImGui::PopID();
            }

            ImGui::EndTable();
        }
    }

    const Model* selectedModel = nullptr;

    for (const Model& model : models)
    {
        if (model.id == selectedModelID)
        {
            selectedModel = &model;
            break;
        }
    }

    if (selectedModel != nullptr)
    {
        ImGui::Separator();

        unsigned int texture = imageManager.GetTexture(brand.id);

        if (texture != 0)
        {
            ImGui::Image(
                (ImTextureID)(intptr_t)texture,
                ImVec2(150.0f, 100.0f)
            );
        }

        ImGui::Text("Vehicle type: %s", typeName.c_str());

        if (!selectedModel->name.empty())
        {
            ImGui::Text("Model: %s", selectedModel->name.c_str());
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
            ImGui::Text("Power: %d kW", selectedModel->PowerKW());
        }
        else
        {
            ImGui::Text("Power: %d PS", selectedModel->powerPS);
        }

        ImGui::Separator();

        if (ImGui::Button("Edit model"))
        {
            editWindow.Show(
                brand,
                city,
                country,
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
            if (modelRepository.Remove(selectedModelID))
            {
                errorMessage.clear();
                RefreshModels();
            }
            else
            {
                errorMessage = modelRepository.GetLastError();
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
