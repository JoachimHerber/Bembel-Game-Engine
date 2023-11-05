module;
#include <lodepng/lodepng.h>
module bembel.base;

namespace bembel::base {

bool Image::load(In<std::filesystem::path> file, In<bool> invert_y_axis) {
    std::string const file_path_str = file.string(); // file.c_str() returns a wchar*

    std::vector<unsigned char> data;
    unsigned int               w, h;
    unsigned                   error = lodepng::decode(data, w, h, file_path_str);

    if(error != 0) {
        logError("Can't load file '{}'\n{}", file_path_str, lodepng_error_text(error));
        return false;
    }

    // copy data
    m_width        = w;
    m_height       = h;
    m_num_channels = 4;
    m_data.resize(w * h * 4);

    if(invert_y_axis) {
        for(unsigned int y = 0; y < h; ++y) {
            for(unsigned int x = 0; x < w; ++x) {
                m_data[4 * (x + w * (y)) + 0] = data[4 * (x + w * (h - y - 1)) + 0];
                m_data[4 * (x + w * (y)) + 1] = data[4 * (x + w * (h - y - 1)) + 1];
                m_data[4 * (x + w * (y)) + 2] = data[4 * (x + w * (h - y - 1)) + 2];
                m_data[4 * (x + w * (y)) + 3] = data[4 * (x + w * (h - y - 1)) + 3];
            }
        }
    } else {
        m_data = data;
    }
    return true;
}

bool Image::save(In<std::filesystem::path> file, In<bool> invert_y_axis) {
    std::string const          file_path = file.string(); // file.c_str() returns a wchar*
    std::vector<unsigned char> data;
    unsigned int const&        n = m_num_channels;
    unsigned int const&        h = m_height;
    unsigned int const&        w = m_width;
    if(invert_y_axis) {
        data.resize(m_data.size());
        for(unsigned int y = 0; y < h; ++y) {
            for(unsigned int x = 0; x < w; ++x) {
                for(unsigned int c = 0; c < n; ++c) {
                    data[n * (x + w * (y)) + c] = m_data[n * (x + w * (h - y - 1)) + c];
                }
            }
        }
    } else {
        data = m_data;
    }

    unsigned error = 0;
    switch(n) {
        case 1: error = lodepng::encode(file_path, data, w, h, LCT_GREY); break;
        case 3: error = lodepng::encode(file_path, data, w, h, LCT_RGB); break;
        case 4: error = lodepng::encode(file_path, data, w, h, LCT_RGBA); break;
        default: return false;
    }

    if(error != 0) {
        logError("Can't load file '{}'\n{}", file_path, lodepng_error_text(error));
        return false;
    }
    return true;
}

} // namespace bembel::base
