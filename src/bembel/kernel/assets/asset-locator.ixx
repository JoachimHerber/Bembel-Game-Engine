module;
#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>
export module bembel.kernel.assets:AssetLocator;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

export class AssetLocator final {
  private:
    AssetLocator()  = default;
    ~AssetLocator() = default;

  public:
    static AssetLocator& getInstance();

    bool init(xml::Element const* properties);

    template <typename AssetType>
    void addAssetDirectory(In<std::filesystem::path> directory) {
        return addAssetDirectory(AssetType::ASSET_TYPE_NAME, directory);
    }

    void addAssetDirectory(std::string_view asset_type_name, In<std::filesystem::path> directory);

    void addGenericAssetDirectory(In<std::filesystem::path> directory);

    template <typename AssetType>
    std::optional<std::filesystem::path> findAssetLocation(In<std::string_view> file_name) {
        return findAssetLocation(AssetType::ASSET_TYPE_NAME, file_name);
    }

    std::optional<std::filesystem::path> findAssetLocation(
        const std::string_view asset_type_name, std::string_view file_name
    );

  private:
    std::optional<std::filesystem::path> locateFile(
        std::vector<std::filesystem::path> const& directories, std::string_view file_name
    );

  private:
    Dictionary<std::vector<std::filesystem::path>> m_type_specivic_asset_directories;
    std::vector<std::filesystem::path>             m_generic_asset_directories;
};

} // namespace bembel::kernel
