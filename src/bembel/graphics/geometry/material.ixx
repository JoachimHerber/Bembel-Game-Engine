module;
#include<string_view>
#include<vector>
export module bembel.graphics.geometry:Material;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class Material final {
  public:
    Material(uint renderer, int);
    Material(Material const&) = delete;
    Material(Material&&)      = delete;
    ~Material();

    uint getRenderer() const { return m_renderer; }
    uint getUniformBufferObject() const { return m_uniform_buffer_object; }

    std::vector<Asset<Texture>> textures;

    static constexpr std::string_view ASSET_TYPE_NAME = "Material";

  private:
    uint m_renderer              = 0;
    uint m_uniform_buffer_object = 0;
};

} // namespace bembel::graphics