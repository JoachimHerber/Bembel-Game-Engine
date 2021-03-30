#ifndef BEMBEL_KERNEL_DISPLAY_CURSOR_ICON_HPP
#define BEMBEL_KERNEL_DISPLAY_CURSOR_ICON_HPP

#include <bembel/kernel/assets/serial-asset-loader.hpp>

struct GLFWcursor;

namespace bembel::kernel {

class BEMBEL_API CursorIcon {
  public:
    using DefaultLoaderType = SerialAssetLoader<CursorIcon>;

  public:
    CursorIcon(GLFWcursor*);
    ~CursorIcon();

    GLFWcursor* getCursor();

    const static std::string& getTypeName();

    static std::unique_ptr<CursorIcon> loadAsset(AssetManager& asset_mgr, const std::string& path);
    static std::unique_ptr<CursorIcon> createAsset(
        AssetManager& asset_mgr, const base::xml::Element* properties);

    static void deleteAsset(AssetManager&, std::unique_ptr<CursorIcon>) {}

  private:
    GLFWcursor* cursor_imp;
};

struct BEMBEL_API SetCursorIconEvent {
    CursorIcon* const cursor;
    unsigned          window_id;
};

} // namespace bembel::kernel
#endif // include guards
