#include "./image.hpp"

#include <lodepng/lodepng.h>

#include "../logging/logger.hpp"

namespace bembel::base {

Image::Image()
: width(0)
, height(0)
, num_channels(0) {
}

Image::Image(unsigned w, unsigned h, unsigned c)
: width(w)
, height(h)
, num_channels(c) {
    this->data.resize(this->width * this->height * this->num_channels);
}

Image::~Image() {
}

unsigned char* Image::getData() {
    return &(this->data[0]);
}

const unsigned char* Image::getData() const {
    return &(this->data[0]);
}

unsigned int Image::getWidth() const {
    return this->width;
}

unsigned int Image::getHeight() const {
    return this->height;
}

unsigned int Image::getChannels() const {
    return this->num_channels;
}

bool Image::load(const std::string& file_name, bool invert_y_axis) {
    std::vector<unsigned char> data;
    unsigned int w, h;
    unsigned error = lodepng::decode(data, w, h, file_name);

    if(error != 0) {
        BEMBEL_LOG_ERROR() << "Can't load file \"" << file_name << "\"\n"
                           << lodepng_error_text(error) << std::endl;
        return false;
    }

    // copy data
    this->width        = w;
    this->height       = h;
    this->num_channels = 4;
    this->data.resize(w * h * 4);

    if(invert_y_axis) {
        for(unsigned int y = 0; y < h; ++y) {
            for(unsigned int x = 0; x < w; ++x) {
                this->data[4 * (x + w * (y)) + 0] = data[4 * (x + w * (h - y - 1)) + 0];
                this->data[4 * (x + w * (y)) + 1] = data[4 * (x + w * (h - y - 1)) + 1];
                this->data[4 * (x + w * (y)) + 2] = data[4 * (x + w * (h - y - 1)) + 2];
                this->data[4 * (x + w * (y)) + 3] = data[4 * (x + w * (h - y - 1)) + 3];
            }
        }
    } else {
        this->data = data;
    }
    return true;
}

bool Image::save(const std::string& file_name, bool invert_y_axis) {
    std::vector<unsigned char> data;
    const unsigned int& n = this->num_channels;
    const unsigned int& h = this->height;
    const unsigned int& w = this->width;
    if(invert_y_axis) {
        data.resize(this->data.size());
        for(unsigned int y = 0; y < h; ++y) {
            for(unsigned int x = 0; x < w; ++x) {
                for(unsigned int c = 0; c < n; ++c) {
                    data[n * (x + w * (y)) + c] = this->data[n * (x + w * (h - y - 1)) + c];
                }
            }
        }
    } else {
        data = this->data;
    }

    unsigned error = 0;
    switch(n) {
        case 1: error = lodepng::encode(file_name, data, w, h, LCT_GREY); break;
        case 3: error = lodepng::encode(file_name, data, w, h, LCT_RGB); break;
        case 4: error = lodepng::encode(file_name, data, w, h, LCT_RGBA); break;
        default: return false;
    }

    if(error != 0) {
        BEMBEL_LOG_ERROR() << "Can't load file \"" << file_name << "\"\n"
                           << lodepng_error_text(error) << std::endl;
        return false;
    }
    return true;
}

} // namespace bembel::base
