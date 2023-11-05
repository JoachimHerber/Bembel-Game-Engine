module;
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
module bembel.kernel.rendering;

import bembel.base;
import bembel.kernel.assets;

namespace bembel::kernel {
using namespace bembel::base;

bool TextureAtlas::loadTexture(std::filesystem::path file) {
    base::Image image;
    if(!image.load(file)) return false;

    m_main_texture->init(image);
    return true;
}

std::unique_ptr<TextureAtlas> TextureAtlas::loadAsset(std::filesystem::path file) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        logError("Failed to load file '{}' \n {}", file_path, doc.ErrorName());
        return nullptr;
    }

    xml::Element const* root = doc.FirstChildElement("TextureAtlas");
    if(!root) {
        logError("File '{}' has no root element 'TextureAtlas'", file_path);
        return nullptr;
    }
    auto atlas = TextureAtlas::createAsset(root);
    if(!atlas) { logError("Failed to create TextureAtlas from file '{}'", file_path); }
    return std::move(atlas);
}

std::unique_ptr<TextureAtlas> TextureAtlas::createAsset(xml::Element const* properties) {
    if(!properties->Attribute("texture")) return nullptr;

    std::string_view file = properties->Attribute("texture");
    auto             path = AssetLocator::getInstance().findAssetLocation<Texture>(file);
    if(!path) { return nullptr; }

    auto atlas = std::make_unique<TextureAtlas>();
    atlas->loadTexture(path.value());
    for(auto it : xml::IterateChildElements(properties, "Entry")) {
        std::string name;
        vec2        min;
        vec2        max;
        if(xml::getAttribute(it, "name", name) && xml::getAttribute(it, "min", min)
           && xml::getAttribute(it, "max", max)) {
            atlas->setEntry(name, min, max);
        }
    }

    return atlas;
}

} // namespace bembel::kernel
