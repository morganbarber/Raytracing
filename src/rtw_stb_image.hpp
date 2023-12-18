#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

// Disable strict warnings for this header from the Microsoft Visual C++ compiler.
#ifdef _MSC_VER
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

class rtw_image {
  public:
    rtw_image() : data(nullptr) {}

    rtw_image(const char* image_filename) {
        // Loads image data from the specified file. If the RTW_IMAGES environment variable is
        // defined, looks only in that directory for the image file. If the image was not found,
        // searches for the specified image file first from the current directory, then in the
        // images/ subdirectory, then the _parent's_ images/ subdirectory, and then _that_
        // parent, on so on, for six levels up. If the image was not loaded successfully,
        // width() and height() will return 0.

        std::vector<std::string> searchPaths;
        auto imagedir = getenv("RTW_IMAGES");
        if (imagedir) {
            searchPaths.push_back(std::string(imagedir) + "/" + image_filename);
        }
        searchPaths.push_back(image_filename);
        searchPaths.push_back("images/" + std::string(image_filename));
        searchPaths.push_back("../images/" + std::string(image_filename));
        searchPaths.push_back("../../images/" + std::string(image_filename));
        searchPaths.push_back("../../../images/" + std::string(image_filename));
        searchPaths.push_back("../../../../images/" + std::string(image_filename));
        searchPaths.push_back("../../../../../images/" + std::string(image_filename));
        searchPaths.push_back("../../../../../../images/" + std::string(image_filename));

        for (const auto& path : searchPaths) {
            if (load(path)) {
                return;
            }
        }

        std::cerr << "ERROR: Could not load image file '" << image_filename << "'.\n";
    }

    ~rtw_image() { STBI_FREE(data); }

    bool load(const std::string& filename) {
        // Loads image data from the given file name. Returns true if the load succeeded.
        int n = bytes_per_pixel; // Dummy out parameter: original components per pixel
        data = stbi_load(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
        bytes_per_scanline = image_width * bytes_per_pixel;
        return data != nullptr;
    }

    int width()  const { return (data == nullptr) ? 0 : image_width; }
    int height() const { return (data == nullptr) ? 0 : image_height; }

    const unsigned char* pixel_data(int x, int y) const {
        // Return the address of the three bytes of the pixel at x,y (or magenta if no data).
        static unsigned char magenta[] = { 255, 0, 255 };
        if (data == nullptr) return magenta;

        x = clamp(x, 0, image_width);
        y = clamp(y, 0, image_height);

        return data + y * bytes_per_scanline + x * bytes_per_pixel;
    }

  private:
    static constexpr int bytes_per_pixel = 3;
    unsigned char* data = nullptr;
    int image_width = 0;
    int image_height = 0;
    int bytes_per_scanline = 0;

    static int clamp(int x, int low, int high) {
        // Return the value clamped to the range [low, high).
        return std::clamp(x, low, high - 1);
    }
};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
    #pragma warning (pop)
#endif

#endif