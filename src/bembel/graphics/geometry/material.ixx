export module bembel.graphics.geometry:Material;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

class GraphicSystem;

export class Material final {
  public:
    using TextureAsset = Asset<Texture>;
    using Textures     = std::vector<TextureAsset>;

  public:
    Material(uint renderer, int);
    Material(Material const&) = delete;
    Material(Material&&)      = delete;
    ~Material();

    unsigned getRenderer() const { return m_renderer; }
    uint     getUniformBufferObject() const { return m_uniform_buffer_object; }

    Textures const& getTextures() const { return m_textures; }
    void            setTextures(Textures const& textures) { m_textures = textures; }
    void            setTextures(Textures&& textures) { m_textures = textures; }

    static constexpr std::string_view ASSET_TYPE_NAME = "Material";

  private:
    unsigned m_renderer = 0;

    uint m_uniform_buffer_object;

    Textures m_textures;
};

} // namespace bembel::graphics