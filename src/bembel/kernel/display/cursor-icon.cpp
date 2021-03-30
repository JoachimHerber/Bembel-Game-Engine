#include "./cursor-icon.hpp"

#include <bembel/base/io/image.hpp>

namespace bembel::kernel {

CursorIcon::CursorIcon(GLFWcursor* cursor_imp)
: cursor_imp{cursor_imp} {
}

CursorIcon::~CursorIcon() {
}

GLFWcursor* CursorIcon::getCursor() {
    return this->cursor_imp;
}

const std::string& CursorIcon::getTypeName() {
    const static std::string type_name = "CursorIcon";
    return type_name;
}

std::unique_ptr<CursorIcon> CursorIcon::loadAsset(
    AssetManager& asset_mgr, const std::string& path) {
    return nullptr;
}

std::unique_ptr<CursorIcon> CursorIcon::createAsset(
    AssetManager& asset_mgr, const base::xml::Element* properties) {
    return nullptr;
}

} // namespace bembel::kernel
