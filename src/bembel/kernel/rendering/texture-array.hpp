#ifndef BEMBEL_KERNEL_RENDERING_TEXTURE_ARRAY_HPP
#define BEMBEL_KERNEL_RENDERING_TEXTURE_ARRAY_HPP

#include <bembel/base/io/image.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/assets/serial-asset-loader.hpp>
#include <bembel/kernel/open-gl.hpp>

namespace bembel::kernel {

class BEMBEL_API TextureArray final {
 public:
  using DefaultLoaderType = SerialAssetLoader<TextureArray>;

 public:
  TextureArray(GLenum format = GL_RGBA8);
  ~TextureArray();

  void init(
    const glm::uvec3& size,
    GLenum min_filter = GL_LINEAR,
    GLenum mag_filter = GL_LINEAR,
    GLenum warp_s     = GL_CLAMP_TO_EDGE,
    GLenum warp_t     = GL_CLAMP_TO_EDGE);
  bool init(
    const std::vector<base::Image>& image,
    GLenum min_filter = GL_LINEAR,
    GLenum mag_filter = GL_LINEAR,
    GLenum warp_s     = GL_CLAMP_TO_EDGE,
    GLenum warp_t     = GL_CLAMP_TO_EDGE);
  bool init(
    const std::vector<base::Image>& image,
    const std::unordered_map<std::string, int>& textures_names,
    GLenum min_filter = GL_LINEAR,
    GLenum mag_filter = GL_LINEAR,
    GLenum warp_s     = GL_CLAMP_TO_EDGE,
    GLenum warp_t     = GL_CLAMP_TO_EDGE);
  void cleanup();

  int getTextureIndex(std::string& name);

  void use() const;

  GLuint getTextureHandle() const;
  GLenum getTextureFormat() const;
  GLenum getTextureTarget() const;

  const static std::string& getTypeName();

  static std::unique_ptr<TextureArray> loadAsset(
    AssetManager* asset_manager, const std::string& file_name);
  static std::unique_ptr<TextureArray> createAsset(
    AssetManager* asset_manager, const xml::Element* properties);

  static void deleteAsset(
    AssetManager* asset_manager, std::unique_ptr<TextureArray> texture);

 private:
  GLenum _format;
  GLuint _handle;
  glm::ivec3 _size;

  std::unordered_map<std::string, int> _textures_names;
};

} // namespace bembel::kernel
#endif // include guards
