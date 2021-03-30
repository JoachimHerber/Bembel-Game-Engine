#include "./asset-locator.hpp"

namespace bembel::kernel {

AssetLocator::AssetLocator() {
}

AssetLocator::~AssetLocator() {
}

bool AssetLocator::init(const xml::Element* properties) {
    for(auto it : base::xml::IterateChildElements(properties, "AssetDirectory")) {
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
    const std::string& assetTypeName, const std::string& directory) {
    this->type_specivic_asset_directories[assetTypeName].push_back(directory);
}

void AssetLocator::addGenericAssetDirectory(const std::string& directory) {
    this->generic_asset_directories.push_back(directory);
}

namespace {
    bool locateFile(
        const std::vector<std::string>& directories,
        const std::string& fileName,
        std::string* fullFilePath) {
        for(const std::string& dir : directories) {
            std::string path = dir + "/" + fileName;
            std::ifstream infile(path.c_str());
            if(infile.good()) {
                if(fullFilePath != nullptr) *fullFilePath = path;

                return true;
            }
        }
        return false;
    }
} // namespace

bool AssetLocator::findAssetLocation(
    const std::string& assetTypeName, const std::string& fileName, std::string* fullFilePath) {
    auto it = this->type_specivic_asset_directories.find(assetTypeName);
    if(it != this->type_specivic_asset_directories.end()) {
        if(locateFile(it->second, fileName, fullFilePath)) return true;
    }
    return locateFile(this->generic_asset_directories, fileName, fullFilePath);
}

} // namespace bembel::kernel
