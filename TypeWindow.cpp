#include "TypeWindow.h"
#include "DetailedWindow.h"
#include "imgui.h"

#include <cstdio>

TypeWindow::TypeWindow(Database& database, DetailedWindow& detailedWindow)
    : Window("Vehicle Types"),
    db(database),
    detailedWindow(detailedWindow)
{
}

void TypeWindow::Show(int brandID, const std::string& name)
{
    selectedBrandID = brandID;
    selectedTypeID = -1;
    brandName = name;
    errorMessage.clear();

    location = db.GetBrandLocation(brandID);

    RefreshTypes();

    Open();
}

void TypeWindow::RefreshTypes()
{
    types = db.GetTypes(selectedBrandID);
}

void TypeWindow::DrawContent()
{
    ImGui::Text("Brand: %s", brandName.c_str());

    ImGui::Text(
        "Headquarters: %s, %s",
        location.city.c_str(),
        location.country.c_str()
    );

    ImGui::Separator();

    ImGui::Text("Add a vehicle type");

    ImGui::InputText(
        "New vehicle type",
        newTypeName,
        sizeof(newTypeName)
    );

    if (ImGui::Button("Add vehicle type"))
    {
        if (db.AddVehicleType(selectedBrandID, newTypeName))
        {
            newTypeName[0] = '\0';
            errorMessage.clear();
            RefreshTypes();
        }
        else
        {
            errorMessage = db.GetLastError();
        }
    }

    ImGui::Separator();

    ImGui::Text("Available vehicle types");

    if (ImGui::BeginTable(
        "VehicleTypesTable",
        1,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Vehicle Type");
        ImGui::TableHeadersRow();

        for (const VehicleType& type : types)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            ImGui::PushID(type.id);

            if (ImGui::Selectable(
                type.name.c_str(),
                selectedTypeID == type.id))
            {
                selectedTypeID = type.id;

                std::snprintf(
                    editedTypeName,
                    sizeof(editedTypeName),
                    "%s",
                    type.name.c_str()
                );

                detailedWindow.Show(selectedBrandID, brandName, type.id);
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }

    if (selectedTypeID != -1)
    {
        ImGui::Separator();

        ImGui::Text("Edit selected vehicle type");

        ImGui::InputText(
            "Vehicle type name",
            editedTypeName,
            sizeof(editedTypeName)
        );

        if (ImGui::Button("Save vehicle type"))
        {
            if (db.UpdateVehicleType(selectedTypeID, editedTypeName))
            {
                errorMessage.clear();
                RefreshTypes();

                detailedWindow.Show(
                    selectedBrandID,
                    brandName,
                    selectedTypeID
                );
            }
            else
            {
                errorMessage = db.GetLastError();
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete vehicle type"))
        {
            if (db.DeleteVehicleType(selectedTypeID))
            {
                selectedTypeID = -1;
                errorMessage.clear();

                detailedWindow.Close();

                RefreshTypes();
            }
            else
            {
                errorMessage = db.GetLastError();
            }
        }
    }

    if (!errorMessage.empty())
    {
        ImGui::TextWrapped("Error: %s", errorMessage.c_str());
    }
}