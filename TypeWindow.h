#pragma once

#include "Window.h"
#include "dal/BrandRepository.h"
#include "dal/CityRepository.h"
#include "dal/CountryRepository.h"
#include "dal/TypeRepository.h"

class DetailedWindow;

class TypeWindow : public Window
{
private:
    BrandRepository& brandRepository;
    TypeRepository& typeRepository;
    CityRepository& cityRepository;
    CountryRepository& countryRepository;
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
        BrandRepository& brandRepository,
        TypeRepository& typeRepository,
        CityRepository& cityRepository,
        CountryRepository& countryRepository,
        DetailedWindow& detailedWindow
    );

    void Show(int brandID);

    void DrawContent() override;
};
