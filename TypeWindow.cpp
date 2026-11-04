#include "TypeWindow.h"
#include "DetailedWindow.h"
#include "imgui.h"

#include <cstdio>
#include <optional>

TypeWindow::TypeWindow(
    IBrandRepository& brandRepository,
    ITypeRepository& typeRepository,
    ICityRepository& cityRepository,
    ICountryRepository& countryRepository,
    DetailedWindow& detailedWindow)
    : Window("Vehicle Types"),
    brandRepository(brandRepository),
    typeRepository(typeRepository),
    cityRepository(cityRepository),
    countryRepository(countryRepository),
    detailedWindow(detailedWindow)
{
}

void TypeWindow::Show(int brandID)
{
    selectedTypeID = -1;
    errorMessage.clear();

    std::optional<Brand> foundBrand = brandRepository.GetByID(brandID);

    if (!foundBrand)
    {
        errorMessage = brandRepository.GetLastError();
        return;
    }

    brand = *foundBrand;

    RefreshLocation();
    RefreshTypes();

    Open();
}

void TypeWindow::RefreshLocation()
{
    city = City{};
    country = Country{};

    if (brand.cityID == 0)
    {
        return;
    }

    std::optional<City> foundCity = cityRepository.GetByID(brand.cityID);

    if (!foundCity)
    {
        errorMessage = cityRepository.GetLastError();
        return;
    }

    city = *foundCity;

    std::optional<Country> foundCountry = countryRepository.GetByID(city.countryID);

    if (!foundCountry)
    {
        errorMessage = countryRepository.GetLastError();
        return;
    }

    country = *foundCountry;
}

void TypeWindow::RefreshTypes()
{
    types = typeRepository.GetByBrand(brand.id);
}

void TypeWindow::OpenDetails(int typeID)
{
    for (const VehicleType& type : types)
    {
        if (type.id == typeID)
        {
            detailedWindow.Show(brand, city, country, type);
            return;
        }
    }
}

void TypeWindow::DrawContent()
{
    ImGui::Text("Brand: %s", brand.name.c_str());

    ImGui::Text(
        "Headquarters: %s, %s",
        city.name.c_str(),
        country.name.c_str()
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
        if (typeRepository.Add(brand.id, newTypeName))
        {
            newTypeName[0] = '\0';
            errorMessage.clear();
            RefreshTypes();
        }
        else
        {
            errorMessage = typeRepository.GetLastError();
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

                OpenDetails(type.id);
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
            if (typeRepository.Rename(selectedTypeID, editedTypeName))
            {
                errorMessage.clear();
                RefreshTypes();
                OpenDetails(selectedTypeID);
            }
            else
            {
                errorMessage = typeRepository.GetLastError();
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete vehicle type"))
        {
            if (typeRepository.Remove(selectedTypeID))
            {
                selectedTypeID = -1;
                errorMessage.clear();

                detailedWindow.Close();

                RefreshTypes();
            }
            else
            {
                errorMessage = typeRepository.GetLastError();
            }
        }
    }

    if (!errorMessage.empty())
    {
        ImGui::TextWrapped("Error: %s", errorMessage.c_str());
    }
}
