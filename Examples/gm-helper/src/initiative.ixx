export module bembel.examples.gm_helper:Initiative;

import std;
import bembel;
import :Components;

namespace bembel::examples::gm_helper {
using namespace base;
using namespace kernel;
using namespace graphics;

export std::span<Entity> getInitiativeOrder(Scene* scene) {
    thread_local std::vector<Entity> entities;
    entities.clear();

    ComponentMask mask = scene->getComponentContainer<Initiative>()->getComponentMask()
                       | scene->getComponentContainer<Token>()->getComponentMask();

    auto const& entitis = scene->getEntitys();
    for(usize entity = 0; entity < entitis.size(); ++entity) {
        if((entitis[entity] & mask) != mask) continue;

        entities.emplace_back(scene, EntityID(entity));
    }
    std::sort(entities.begin(), entities.end(), [](Entity a, Entity b) {
        auto* i1 = a.get<Initiative>();
        auto* i2 = b.get<Initiative>();
        return i1->roll > i2->roll || (i1->roll == i2->roll && i1->bonus > i2->bonus);
    });
    return entities;
}

} // namespace bembel::examples::gm_helper