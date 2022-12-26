module;
#include <memory>
#include <string>
#include <string_view>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
module bembel.kernel.assets;

import bembel.base;

import :CursorIcon;

namespace bembel::kernel {
using namespace bembel::base;

void CursorIcon::createStandardCursors() {
    assets::registerAssetType<CursorIcon>();

    auto createStandardCursor = [&](int shape, std::string_view name) {
        auto cursor = glfwCreateStandardCursor(shape);
        if(cursor) {
            Asset<CursorIcon>::getContainer()->addAsset(
                std::make_unique<CursorIcon>(cursor, name), name
            );
        } else {
            log().error("Failed to create StandardCursor {}", name);
        }
    };

    createStandardCursor(GLFW_ARROW_CURSOR, "Arrow");
    createStandardCursor(GLFW_IBEAM_CURSOR, "IBeam");
    createStandardCursor(GLFW_CROSSHAIR_CURSOR, "Crosshair");
    createStandardCursor(GLFW_HAND_CURSOR, "Hand");
    createStandardCursor(GLFW_HRESIZE_CURSOR, "HResize");
    createStandardCursor(GLFW_VRESIZE_CURSOR, "VResize");
    // @ToDo glfw 3.4 Cursors
    // createStandardCursor(GLFW_RESIZE_EW_CURSOR, "RESIZE_EW");
    // createStandardCursor(GLFW_RESIZE_NS_CURSOR, "RESIZE_NS");
    // createStandardCursor(GLFW_RESIZE_NWSE_CURSOR, "RESIZE_NWSE");
    // createStandardCursor(GLFW_RESIZE_NESW_CURSOR, "RESIZE_NESW");
}

std::unique_ptr<CursorIcon> CursorIcon::loadAsset(std::filesystem::path path) {
    return nullptr;
}

std::unique_ptr<CursorIcon> CursorIcon::createAsset(xml::Element const* properties) {
    return nullptr;
}

} // namespace bembel::kernel
