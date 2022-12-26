module;
#include <memory>
#include <string_view>
export module bembel.kernel.rendering:TextureAtlas;

import bembel.base;
import bembel.kernel.assets;
import :Texture;

namespace bembel::kernel {
using namespace bembel::base;

export class TextureAtlas final {
  public:
    TextureAtlas()  = default;
    ~TextureAtlas() = default;

    bool loadTexture(std::string_view file);

    Texture* getMainTexture() const { return m_main_texture.get(); }
    Texture* getLooupTexture() const { return m_tex_coord_looup_texture.get(); }

    struct Entry {
        vec2 min;
        vec2 max;
    };

    Entry const* getEntry(std::string_view name) const {
        auto entry = m_entries.find(name);
        if(entry != m_entries.end()) return &(entry->second);

        return nullptr;
    }
    void setEntry(std::string_view name, vec2 const& min, vec2 const& max) {
        m_entries.emplace(name, Entry(min, max));
    }

    static constexpr std::string_view ASSET_TYPE_NAME = "TextureAtlas";

    static std::unique_ptr<TextureAtlas> loadAsset(
         std::filesystem::path file_name
    );
    static std::unique_ptr<TextureAtlas> createAsset(
        xml::Element const* properties
    );

    using DefaultLoaderType = SerialAssetLoader<TextureAtlas>;

  private:
    using Format = Texture::Format;
    using Target = Texture::Target;

    std::unique_ptr<Texture> m_main_texture = std::make_unique<Texture>();
    std::unique_ptr<Texture> m_tex_coord_looup_texture =
        std::make_unique<Texture>(Target::TEXTURE_1D, Format::RGBA16);

    Dictionary<Entry> m_entries;
};

} // namespace bembel::kernel
