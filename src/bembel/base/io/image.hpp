#ifndef BEMBEL_BASE_IO_IMAGE_HPP
#define BEMBEL_BASE_IO_IMAGE_HPP

#include <bembel/library.hpp>

namespace bembel::base {

class BEMBEL_API Image {
  public:
    Image();
    Image(unsigned int w, unsigned int h, unsigned int c);
    ~Image();

    unsigned char* getData();
    const unsigned char* getData() const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getChannels() const;

    bool load(const std::string& file_name, bool invert_y_axis = true);
    bool save(const std::string& file_name, bool invert_y_axis = true);

  private:
    std::vector<unsigned char> data;

    unsigned int width;
    unsigned int height;
    unsigned int num_channels;
};

} // namespace bembel::base
#endif // include guard
