module;
#include <filesystem>
#include <string_view>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
export module bembel.kernel.assets:CursorIcon;

import bembel.base;

import :SerialAssetLoader;

using namespace bembel::base;

namespace bembel::kernel {

export class CursorIcon {
  public:
    CursorIcon(GLFWcursor* cursor_imp, std::string_view name)
      : m_cursor_imp{cursor_imp}, m_name{name} {}
    ~CursorIcon() = default;

    GLFWcursor*      getCursor() { return m_cursor_imp; }
    std::string_view getName() { return m_name; }

    static constexpr std::string_view ASSET_TYPE_NAME = "CursorIcon";

    static void createStandardCursors();

    static std::unique_ptr<CursorIcon> loadAsset(std::filesystem::path path);
    static std::unique_ptr<CursorIcon> createAsset(xml::Element const* properties);

    using DefaultLoaderType = SerialAssetLoader<CursorIcon>;

  private:
    GLFWcursor* m_cursor_imp;

    std::string_view m_name;
};

export struct SetCursorIconEvent {
    CursorIcon* cursor;
    WindowId    window_id;
};

export enum class CursorMode { NORMAL, HIDDEN, DISABLED };

export struct SetCursorModeEvent {
    CursorMode mode;
    WindowId   window_id;
};

} // namespace bembel::kernel
