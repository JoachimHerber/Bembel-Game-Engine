module;
#include <limits>
#include <optional>
#include <string_view>
export module bembel.examples.gm_helper:Selection;

import bembel;
import :Components;

namespace bembel::examples::gm_helper {
using namespace base;
using namespace kernel;

std::optional<Entity> g_selected_entity;

export std::optional<Entity> getSelection() {
    return g_selected_entity;
}

export void updateSelection(In<std::optional<Entity>> entity) {
    if(g_selected_entity == entity) return;

    if(g_selected_entity) { g_selected_entity->get<Selectable>()->selected = false; }
    g_selected_entity = entity;
    if(g_selected_entity) { //
        g_selected_entity->get<Selectable>()->selected = true;
    }
}

export void updateSelection(In<vec2> cursor_pos, In<Scene*> scene) {
    auto* transform_components  = scene->getComponentContainer<Transform>();
    auto* selectable_components = scene->getComponentContainer<Selectable>();

    ComponentMask mask =
        transform_components->getComponentMask() | selectable_components->getComponentMask();

    auto const& entitis     = scene->getEntitys();
    auto const& selectables = selectable_components->getComponentData();
    auto        transform   = transform_components->begin();

    EntityID selected = EntityID::INVALID;
    float    min_dist = std::numeric_limits<float>::infinity();

    for(usize entity = 0; entity < entitis.size(); ++entity, ++transform) {
        if((entitis[entity] & mask) != mask) continue;

        auto  div  = vec2(transform->position) - cursor_pos;
        float dist = glm::length(div) / selectables[entity].dist;
        if(dist < min_dist) {
            selected = EntityID(entity);
            min_dist = dist;
        }
    }
    if(min_dist < 1.0f) {
        updateSelection(Entity{scene, selected});
    } else {
        updateSelection({});
    }
}

} // namespace bembel::examples::gm_helper