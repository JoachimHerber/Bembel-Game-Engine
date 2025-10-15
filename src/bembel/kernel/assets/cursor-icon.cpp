module bembel.kernel.assets;

import std;
import glfw;
import bembel.base;

import :CursorIcon;

namespace bembel::kernel {
using namespace bembel::base;

void CursorIcon::createStandardCursors() {
    assets::registerAssetType<CursorIcon>();

    auto createStandardCursor = [&](int shape, std::string_view name) {
        auto cursor = glfw::createStandardCursor(shape);
        if(cursor) {
            Asset<CursorIcon>::getContainer()->addAsset(
                std::make_unique<CursorIcon>(cursor, name), name
            );
        } else {
            logError("Failed to create StandardCursor {}", name);
        }
    };

    createStandardCursor(glfw::ARROW_CURSOR, "Arrow");
    createStandardCursor(glfw::IBEAM_CURSOR, "IBeam");
    createStandardCursor(glfw::CROSSHAIR_CURSOR, "Crosshair");
    createStandardCursor(glfw::HAND_CURSOR, "Hand");
    createStandardCursor(glfw::HRESIZE_CURSOR, "HResize");
    createStandardCursor(glfw::VRESIZE_CURSOR, "VResize");
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
