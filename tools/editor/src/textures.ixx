module;
#include <filesystem>
#include <memory>
#include <string_view>
#include <vector>
export module bembel.tools.editor:Textures;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::graphics;

export struct TextureData {
    std::string    name;
    Asset<Texture> asset;
};

export std::vector<TextureData> TEXTURES;

export void drawTextureManagementUI();

export bool drawTextureSelectionUI(In<char const*> label, InOut<std::optional<uint>> index);

} // namespace bembel::tools