export module bembel.examples.gm_helper:Maps;

import std;
import bembel;
import :Components;

namespace bembel::examples::gm_helper {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export struct Map {
    std::filesystem::path  file;
    std::shared_ptr<Scene> scene;

    void save() { scene->saveScene(file); }
};
export std::vector<Map> g_maps;
export uint             g_current_map = 0;

export std::shared_ptr<Scene> const& getCurrentScene() {
    return g_maps[g_current_map].scene;
}

export bool loadMaps(In<std::filesystem::path> dir) {
    for(auto const& it : std::filesystem::directory_iterator{dir}) {
        if(!it.is_regular_file()) continue;
        auto path      = it.path();
        auto stem      = path.stem();
        auto extension = path.extension();
        if(extension != ".xml") continue;

        auto scene = std::make_shared<Scene>();
        scene->registerComponentTypes<
            Transform,
            AreaMarker,
            Sprite,
            Selectable,
            Token,
            Light,
            Initiative>();
        if(scene->loadScene(path)) { g_maps.emplace_back(path, std::move(scene)); }
    }
    return g_maps.size() > 0;
}

} // namespace bembel::examples::gm_helper