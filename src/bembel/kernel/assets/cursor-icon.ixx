module;
#include "bembel/pch.h"
export module bembel.kernel.assets:CursorIcon;

import glfw;

import bembel.base;

import :AssetManager;
import :SerialAssetLoader;

using namespace bembel::base;

namespace bembel::kernel {

export class CursorIcon {
  public:
    CursorIcon(GLFWcursor* cursor_imp, std::string_view name)
      : m_cursor_imp{cursor_imp}
      , m_name{name} {}
    ~CursorIcon() = default;

    GLFWcursor*      getCursor() { return m_cursor_imp; }
    std::string_view getName() { return m_name; }

    static constexpr std::string_view ASSET_TYPE_NAME = "CursorIcon";

    static void createStandardCursors(AssetManager& asset_mgr);

    static std::unique_ptr<CursorIcon> loadAsset(
        AssetManager& asset_mgr, std::filesystem::path path
    );
    static std::unique_ptr<CursorIcon> createAsset(
        AssetManager& asset_mgr, xml::Element const* properties
    );

    static void deleteAsset(AssetManager&, std::unique_ptr<CursorIcon>) {}

    using DefaultLoaderType = SerialAssetLoader<CursorIcon>;

  private:
    GLFWcursor* m_cursor_imp;

    std::string_view m_name;
};

export struct SetCursorIconEvent {
    CursorIcon* const cursor;
    WindowId          window_id;
};

} // namespace bembel::kernel
