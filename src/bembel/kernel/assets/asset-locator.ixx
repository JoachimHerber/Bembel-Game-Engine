module;
#include <fstream>
#include <string_view>
#include <vector>
export module bembel.kernel.assets:AssetLocator;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

export class AssetLocator final {
  public:
    AssetLocator()  = default;
    ~AssetLocator() = default;

    bool init(xml::Element const* properties) {
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

    template <typename AssetType>
    void addAssetDirectory(std::string_view directory) {
        return addAssetDirectory(AssetType::ASSET_TYPE_NAME, directory);
    }

    void addAssetDirectory(std::string_view asset_type_name, std::string_view directory) {
        auto it = m_type_specivic_asset_directories.find(asset_type_name);
        if(it != m_type_specivic_asset_directories.end()) {
            it->second.emplace_back(directory);
        } else {
            std::vector<std::string> dirs{};
            dirs.emplace_back(directory);
            m_type_specivic_asset_directories.emplace(asset_type_name, std::move(dirs));
        }
    }

    void addGenericAssetDirectory(std::string_view directory) {
        m_generic_asset_directories.emplace_back(directory);
    }

    template <typename AssetType>
    bool findAssetLocation(std::string_view file_name, std::string* full_file_path) {
        return findAssetLocation(AssetType::ASSET_TYPE_NAME, file_name, full_file_path);
    }

    bool findAssetLocation(
        const std::string_view asset_type_name,
        std::string_view       file_name,
        std::string*           full_file_path
    ) {
        auto it = m_type_specivic_asset_directories.find(asset_type_name);
        if(it != m_type_specivic_asset_directories.end()) {
            if(locateFile(it->second, file_name, full_file_path)) return true;
        }
        return locateFile(m_generic_asset_directories, file_name, full_file_path);
    }

  private:
    bool locateFile(
        std::vector<std::string> const& directories,
        std::string_view                fileName,
        std::string*                    full_file_path
    ) {
        for(std::string const& dir : directories) {
            std::string path = dir + "/";
            path += fileName;
            std::ifstream infile(path.c_str());
            if(infile.good()) {
                if(full_file_path != nullptr) *full_file_path = path;

                return true;
            }
        }
        return false;
    }

  private:
    Dictionary<std::vector<std::string>> m_type_specivic_asset_directories;
    std::vector<std::string>             m_generic_asset_directories;
};

} // namespace bembel::kernel
