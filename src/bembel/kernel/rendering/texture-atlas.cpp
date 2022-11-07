module;
#include "bembel/pch.h"
module bembel.kernel.rendering;

import bembel.base;
import bembel.kernel.assets;

namespace bembel::kernel {
using namespace bembel::base;

std::unique_ptr<TextureAtlas> TextureAtlas::loadAsset(
    AssetManager& asset_mgr, std::filesystem::path file
) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        log().error("Failed to load file '{}' \n {}", file_path, doc.ErrorName());
        return nullptr;
    }

    xml::Element const* root = doc.FirstChildElement("TextureAtlas");
    if(!root) {
        log().error("File '{}' has no root element 'TextureAtlas'", file_path);
        return nullptr;
    }
    auto atlas = TextureAtlas::createAsset(asset_mgr, root);
    if(!atlas) { log().error("Failed to create TextureAtlas from file '{}'", file_path); }
    return std::move(atlas);
}

std::unique_ptr<TextureAtlas> TextureAtlas::createAsset(
    AssetManager& asset_mgr, xml::Element const* properties
) {
    if(!properties->Attribute("texture")) return nullptr;

    std::string texture;
    if(!asset_mgr.getAssetLocator().findAssetLocation<Texture>(
           properties->Attribute("texture"), &texture
       )) {
        return nullptr;
    }

    auto atlas = std::make_unique<TextureAtlas>();
    atlas->loadTexture(texture);

    for(auto it : base::xml::IterateChildElements(properties, "Entry")) {
        std::string name;
        vec2        min;
        vec2        max;
        if(base::xml::getAttribute(it, "name", name) && base::xml::getAttribute(it, "min", min)
           && base::xml::getAttribute(it, "max", max)) {
            atlas->setEntry(name, min, max);
        }
    }

    return atlas;
}

void TextureAtlas::deleteAsset(AssetManager& asset_mgr, std::unique_ptr<TextureAtlas> mesh) {}

} // namespace bembel::kernel
