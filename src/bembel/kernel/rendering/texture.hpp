#ifndef BEMBEL_KERNEL_RENDERING_TEXTURE_HPP
#define BEMBEL_KERNEL_RENDERING_TEXTURE_HPP

#include <bembel/base/io/image.hpp>
#include <bembel/kernel/assets/serial-asset-loader.hpp>
#include <bembel/kernel/open-gl.hpp>

namespace bembel::kernel {

class BEMBEL_API Texture final {
  public:
    using DefaultLoaderType = kernel::SerialAssetLoader<Texture>;

  public:
    Texture(GLenum target = GL_TEXTURE_2D, GLenum format = GL_RGBA8);
    ~Texture();

    void init(
        GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR,
        GLenum mag_filter = GL_LINEAR,
        GLenum warp_s     = GL_CLAMP_TO_EDGE,
        GLenum warp_t     = GL_CLAMP_TO_EDGE);
    void init(
        const glm::uvec2& size,
        GLenum            min_filter = GL_LINEAR_MIPMAP_LINEAR,
        GLenum            mag_filter = GL_LINEAR,
        GLenum            warp_s     = GL_CLAMP_TO_EDGE,
        GLenum            warp_t     = GL_CLAMP_TO_EDGE);
    void init(
        const base::Image& _data,
        GLenum             min_filter = GL_LINEAR_MIPMAP_LINEAR,
        GLenum             mag_filter = GL_LINEAR,
        GLenum             warp_s     = GL_CLAMP_TO_EDGE,
        GLenum             warp_t     = GL_CLAMP_TO_EDGE);
    void cleanup();

    void bind() const;
    void release() const;

    bool setSize(const glm::ivec2& size);
    bool setData(const base::Image&, GLint mip_map_level = 0);

    GLuint getTextureHandle() const;
    GLenum getTextureFormat() const;
    GLenum getTextureTarget() const;

    const static std::string& getTypeName();

    static std::unique_ptr<Texture> loadAsset(
        kernel::AssetManager& asset_mgr, const std::string& file_name);
    static std::unique_ptr<Texture> createAsset(
        kernel::AssetManager& asset_mgr, const xml::Element* properties);
    static void deleteAsset(kernel::AssetManager& asset_mgr, std::unique_ptr<Texture> texture);

  private:
    GLenum target;
    GLenum format;
    GLuint handle;
};

} // namespace bembel::kernel
#endif // include guards
