#pragma once

#include "Window.h"
#include "dal/ICityRepository.h"
#include "dal/ICountryRepository.h"
#include "dal/ITypeRepository.h"

class DetailedWindow;

class TypeWindow : public Window
{
private:
    ITypeRepository& typeRepository;
    ICityRepository& cityRepository;
    ICountryRepository& countryRepository;
    DetailedWindow& detailedWindow;

    Brand brand;
    City city;
    Country country;

    std::vector<VehicleType> types;

    int selectedTypeID = -1;

    char newTypeName[128] = {};
    char editedTypeName[128] = {};

    std::string errorMessage;

    void RefreshLocation();
    void RefreshTypes();
    void OpenDetails(int typeID);

public:
    TypeWindow(
        ITypeRepository& typeRepository,
        ICityRepository& cityRepository,
        ICountryRepository& countryRepository,
        DetailedWindow& detailedWindow
    );

    void Show(const Brand& brand);

    void DrawContent() override;
};
