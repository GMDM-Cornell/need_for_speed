#pragma once

#include "Window.h"
#include "dal/IBrandRepository.h"
#include "ImageManager.h"

class TypeWindow; //Forward declaration, because BrandWindow need TypeWindow without knowing it's content

class BrandWindow : public Window
{
private:
    IBrandRepository& brandRepository;
    ImageManager& imageManager;
    TypeWindow& typeWindow;

    std::vector<Brand> brands;

    int selectedBrandID = -1;

    void RefreshBrands();

public:
    BrandWindow(
        IBrandRepository& brandRepository,
        ImageManager& imageManager,
        TypeWindow& typeWindow
    );

    void DrawContent() override;
};
