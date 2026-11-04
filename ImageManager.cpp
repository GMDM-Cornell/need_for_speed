#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>

#include "ImageManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <limits>

bool ImageManager::LoadLogo(
    int brandID,
    const std::vector<std::uint8_t>& logo)
{
    if (logo.empty()) {
        return false;
    }

    if (logo.size() >
        static_cast<std::size_t>((std::numeric_limits<int>::max)())) {
        return false;
    }

    if (textures.find(brandID) != textures.end()) {
        return true;
    }

    int width = 0;
    int height = 0;
    int channels = 0;

    unsigned char* pixels = stbi_load_from_memory(
        logo.data(),
        static_cast<int>(logo.size()),
        &width,
        &height,
        &channels,
        4
    );

    if (pixels == nullptr) {
        std::cerr << "Logo laden mislukt voor merk-ID "
            << brandID << ": "
            << stbi_failure_reason() << '\n';

        return false;
    }

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP
    );

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels
    );

    stbi_image_free(pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (texture == 0) {
        return false;
    }

    textures[brandID] = texture;
    return true;
}

unsigned int ImageManager::GetTexture(int brandID) const {
    auto it = textures.find(brandID);

    if (it == textures.end()) {
        return 0;
    }

    return it->second;
}

void ImageManager::Clear() {
    for (const auto& item : textures) {
        GLuint texture = item.second;

        if (texture != 0) {
            glDeleteTextures(1, &texture);
        }
    }

    textures.clear();
}
