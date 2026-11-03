
#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

class ImageManager {
private:
    std::unordered_map<int, unsigned int> textures;

public:
    ImageManager() = default;
    ImageManager(const ImageManager&) = delete;
    ImageManager& operator=(const ImageManager&) = delete;

    bool LoadLogo(int brandID, const std::vector<std::uint8_t>& logo);
    unsigned int GetTexture(int brandID) const;
    void Clear();
};
