#include "texture-atlas.hpp"

#include <bembel/base/io/image.hpp>
#include <bembel/kernel/assets/asset-locator.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>

namespace bembel::kernel {

TextureAtlas::TextureAtlas()
: main_texture{std::make_unique<Texture>()}
, tex_coord_looup_texture{std::make_unique<Texture>(GL_TEXTURE_1D, GL_RGBA16)} {
}

TextureAtlas::~TextureAtlas() {
}

bool TextureAtlas::loadTexture(const std::string& file) {
    base::Image image;
    if(!image.load(file)) return false;

    this->main_texture->init(image);
    return true;
}

Texture* TextureAtlas::getMainTexture() const {
    return this->main_texture.get();
}

Texture* TextureAtlas::getLooupTexture() const {
    return this->tex_coord_looup_texture.get();
}

const std::pair<glm::vec2, glm::vec2>* TextureAtlas::getEnty(const std::string& name) const {
    auto entry = this->entries.find(name);
    if(entry != this->entries.end()) return &(entry->second);

    return nullptr;
}

void TextureAtlas::setEnty(const std::string& name, const glm::vec2& min, const glm::vec2& max) {
    this->entries.emplace(name, std::make_pair(min, max));
}

const std::string& TextureAtlas::getTypeName() {
    const static std::string type_name = "TextureAtlas";
    return type_name;
}

std::unique_ptr<TextureAtlas> TextureAtlas::loadAsset(
    AssetManager& asset_mgr, const std::string& file_name) {
    xml::Document doc;
    if(doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) {
        BEMBEL_LOG_ERROR() << "Failed to load file '" << file_name << "'\n"
                           << doc.ErrorName() << std::endl;
        return nullptr;
    }

    const xml::Element* root = doc.FirstChildElement(getTypeName().c_str());
    if(!root) {
        BEMBEL_LOG_ERROR() << "File '" << file_name << "' has no root element 'Shader'"
                           << std::endl;
        return nullptr;
    }
    auto atlas = TextureAtlas::createAsset(asset_mgr, root);
    if(!atlas) {
        BEMBEL_LOG_ERROR() << "Failed to create shader program from file '" << file_name << "'"
                           << std::endl;
    }
    return std::move(atlas);
}

std::unique_ptr<TextureAtlas> TextureAtlas::createAsset(
    AssetManager& asset_mgr, const xml::Element* properties) {
    if(!properties->Attribute("texture")) return nullptr;

    std::string texture;
    if(!asset_mgr.getAssetLocator().findAssetLocation<Texture>(
           properties->Attribute("texture"), &texture)) {
        return nullptr;
    }

    auto atlas = std::make_unique<TextureAtlas>();
    atlas->loadTexture(texture);

    for(auto it : base::xml::IterateChildElements(properties, "Entry")) {
        std::string name;
        glm::vec2   min;
        glm::vec2   max;
        if(base::xml::getAttribute(it, "name", name) && base::xml::getAttribute(it, "min", min)
           && base::xml::getAttribute(it, "max", max)) {
            atlas->setEnty(name, min, max);
        }
    }

    return atlas;
}

void TextureAtlas::deleteAsset(AssetManager& asset_mgr, std::unique_ptr<TextureAtlas> mesh) {
}

} // namespace bembel::kernel
