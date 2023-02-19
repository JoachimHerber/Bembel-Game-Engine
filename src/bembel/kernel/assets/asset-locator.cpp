module;
#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>
module bembel.kernel.assets;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

AssetLocator& AssetLocator::getInstance() {
    static AssetLocator instance;
    return instance;
}

bool AssetLocator::init(xml::Element const* properties) {
    for(auto it : xml::IterateChildElements(properties, "AssetDirectory")) {
        std::string dir, asset_type;
        xml::getAttribute(it, "directory", dir);
        if(xml::getAttribute(it, "asset_type", asset_type))
            addAssetDirectory(asset_type, dir);
        else
            addGenericAssetDirectory(dir);
    }
    return true;
}

void AssetLocator::addAssetDirectory(
    std::string_view asset_type_name, In<std::filesystem::path> directory
) {
    auto it = m_type_specivic_asset_directories.find(asset_type_name);
    if(it != m_type_specivic_asset_directories.end()) {
        it->second.emplace_back(directory);
    } else {
        std::vector<std::filesystem::path> dirs{};
        dirs.emplace_back(directory);
        m_type_specivic_asset_directories.emplace(asset_type_name, std::move(dirs));
    }
}

void AssetLocator::addGenericAssetDirectory(In<std::filesystem::path> directory) {
    m_generic_asset_directories.emplace_back(directory);
}

std::optional<std::filesystem::path> AssetLocator::findAssetLocation(
    const std::string_view asset_type_name, std::string_view file_name
) {
    auto it = m_type_specivic_asset_directories.find(asset_type_name);
    if(it != m_type_specivic_asset_directories.end()) {
        if(auto path = locateFile(it->second, file_name)) return path;
    }
    return locateFile(m_generic_asset_directories, file_name);
}

std::optional<std::filesystem::path> AssetLocator::locateFile(
    std::vector<std::filesystem::path> const& directories, std::string_view file_name
) {
    for(std::filesystem::path path : directories) {
        path.append(file_name);
        if(std::filesystem::exists(path)) { return path; }
    }
    return {};
}

} // namespace bembel::kernel
