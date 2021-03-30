#ifndef BEMBEL_KERNEL_RENDERING_TEXTUREATLAS_HPP
#define BEMBEL_KERNEL_RENDERING_TEXTUREATLAS_HPP

#include "./texture.hpp"

namespace bembel::kernel {

class BEMBEL_API TextureAtlas final {
  public:
    using DefaultLoaderType = SerialAssetLoader<TextureAtlas>;

  public:
    TextureAtlas();
    ~TextureAtlas();

    bool loadTexture(const std::string&);

    Texture* getMainTexture() const;
    Texture* getLooupTexture() const;

    const std::pair<glm::vec2, glm::vec2>* getEnty(const std::string& name) const;
    void setEnty(const std::string& name, const glm::vec2& min, const glm::vec2& max);

    const static std::string& getTypeName();

    static std::unique_ptr<TextureAtlas> loadAsset(
        AssetManager& asset_mgr, const std::string& file_name);
    static std::unique_ptr<TextureAtlas> createAsset(
        AssetManager& asset_mgr, const xml::Element* properties);

    static void deleteAsset(AssetManager& asset_mgr, std::unique_ptr<TextureAtlas> mesh);

  private:
    std::unique_ptr<Texture> main_texture;
    std::unique_ptr<Texture> tex_coord_looup_texture;

    std::map<std::string, std::pair<glm::vec2, glm::vec2>> entries;
};

} // namespace bembel::kernel
#endif // include guard
