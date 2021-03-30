#ifndef BEMBEL_KERNEL_ASSETS_ASSETLOCATOR_HPP
#define BEMBEL_KERNEL_ASSETS_ASSETLOCATOR_HPP

#include <bembel/base/io/xml.hpp>

namespace bembel::kernel {
namespace xml = bembel::base::xml;

class BEMBEL_API AssetLocator final {
  public:
  public:
    AssetLocator();
    ~AssetLocator();

    bool init(const xml::Element* properties);

    template <typename AssetType>
    void addAssetDirectory(const std::string& directory);
    void addAssetDirectory(const std::string& asset_type_name, const std::string& directory);

    void addGenericAssetDirectory(const std::string& directory);

    template <typename AssetType>
    bool findAssetLocation(const std::string& file_name, std::string* full_file_path);
    bool findAssetLocation(
        const std::string& asset_type_name,
        const std::string& file_name,
        std::string* full_file_path);

  private:
    std::unordered_map<std::string, std::vector<std::string>> type_specivic_asset_directories;

    std::vector<std::string> generic_asset_directories;
};

template <typename AssetType>
inline void AssetLocator::addAssetDirectory(const std::string& directory) {
    return this->addAssetDirectory(AssetType::getTypeName(), directory);
}

template <typename AssetType>
inline bool AssetLocator::findAssetLocation(
    const std::string& file_name, std::string* full_file_path) {
    return this->findAssetLocation(AssetType::getTypeName(), file_name, full_file_path);
}

} // namespace bembel::kernel
#endif // include guards
