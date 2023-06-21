module;
#include <vector>
#include <filesystem>
export module bembel.base:Image;

import :Types;

export namespace bembel::base {

class Image {
  public:
    Image() : m_width(0), m_height(0), m_num_channels(0) {}
    Image(In<u32> w, In<u32> h, In<u32> c) : m_width(w), m_height(h), m_num_channels(c) {
        m_data.resize(m_width * m_height * m_num_channels);
    }
    ~Image() = default;

    u8*       getData() { return m_data.data(); }
    u8 const* getData() const { return m_data.data(); }

    u32 getWidth() const { return m_width; }
    u32 getHeight() const { return m_height; }
    u32 getChannels() const { return m_num_channels; }

    bool load(In<std::filesystem::path> file_name, In<bool> invert_y_axis = true);

    bool save(In<std::filesystem::path> file_name, In<bool> invert_y_axis = true);

  private:
    std::vector<u8> m_data;

    u32 m_width;
    u32 m_height;
    u32 m_num_channels;
};

} // namespace bembel::base
