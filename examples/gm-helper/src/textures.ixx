module;
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>
export module bembel.examples.gm_helper:Texture;

import bembel;

namespace bembel::examples::gm_helper {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export std::vector<Asset<Texture>> g_token_textures;
export std::vector<std::string>    g_token_names;

export void loadTokenTextrues(In<std::filesystem::path> dir) {
    for(auto const& it : std::filesystem::directory_iterator{dir}) {
        if(!it.is_regular_file()) continue;
        auto path      = it.path();
        auto stem      = path.stem();
        auto extension = path.extension();
        if(extension != ".png") continue;

        base::Image image;
        if(image.load(path)) {
            auto texture = std::make_unique<Texture>(
                Texture::Target::TEXTURE_2D, Texture::Format::SRGB8_ALPHA8
            );
            texture->init(image);
            g_token_names.push_back(stem.string());
            g_token_textures.push_back(std::move(texture));
        }
    }
}

} // namespace bembel::examples::gm_helper