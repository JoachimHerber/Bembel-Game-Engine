module;
#include <memory>
#include <string_view>
#include <utility>
module bembel.particles;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;
import bembel.graphics.pipeline;

namespace bembel::particles {
using namespace bembel::base;
using namespace bembel::kernel;
class ParticleEffectLoader final : public AssetLoaderBase {
  public:
    using ContainerType = AssetContainer<ParticleEffect>;

    ParticleEffectLoader(ContainerType* container, ParticleSystem* system)
      : m_system(system), m_container(container) {}
    virtual ~ParticleEffectLoader() = default;

    AssetHandle requestAsset(In<std::filesystem::path> file_name) override {
        kernel::AssetHandle handle = m_container->getAssetHandle(file_name.string());

        if(!m_container->isHandelValid(handle)) {
            // we have to load the asset
            std::unique_ptr<ParticleEffect> asset = nullptr;
            //
            if(!asset) return kernel::AssetHandle();

            handle = m_container->addAsset(std::move(asset));
            m_container->incrementAssetRefCount(handle);
            m_container->registerAssetAlias(handle, file_name.string());
        }

        m_container->incrementAssetRefCount(handle);
        return handle;
    }

    AssetHandle requestAsset(xml::Element const* properties) override {
        std::string name = "";
        if(!xml::getAttribute(properties, "name", name)) return AssetHandle();

        AssetHandle handle = m_container->getAssetHandle(name);
        if(!m_container->isHandelValid(handle)) {
            std::string particle_type_name;
            xml::getAttribute(properties, "particle_type", particle_type_name);
            auto type_type = m_system->getParticleTypeIndex(particle_type_name);
            if(!type_type) return AssetHandle();

            uint count = 0;
            xml::getAttribute(properties, "particle_count", count);
            if(!count) return AssetHandle();

            handle = m_container->addAsset(
                std::make_unique<ParticleEffect>(type_type.value(), count, properties)
            );
            m_container->registerAssetAlias(handle, name);
        }

        m_container->incrementAssetRefCount(handle);
        return handle;
    }
    bool releaseAsset(AssetHandle asset_handel) override {
        if(m_container->isHandelValid(asset_handel)) {
            m_container->decrementAssetRefCount(asset_handel);
            if(m_container->getAssetRefCount(asset_handel) == 0) {
                auto mat = m_container->removeAsset(asset_handel);

                return true;
            }
        }
        return false;
    }

    void deleteUnusedAssets() override {
        std::vector<AssetHandle> unusedAssets;
        m_container->getUnusedAssets(unusedAssets);
        for(auto hndl : unusedAssets) { m_container->removeAsset(hndl); }
    }

  private:
    ParticleSystem* m_system;
    ContainerType*  m_container;
};

ParticleSystem::ParticleSystem(In<Engine*> engine) : System("Particles"), m_engine{engine} {
    assets::registerAssetType<ParticleEffect, ParticleEffectLoader>(this);

    RenderingPipeline::Stage::registerStageType<ParticleRenderingStage>("ParticleRenderingStage");
}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::addScene(std::weak_ptr<Scene> scene) {
    if(auto s = scene.lock()) {
        s->registerComponentType<ParticleEmitter>();
        s->registerComponentType<Transform>();
        s->createDataContainer<ParticleData>(m_particle_types);
        m_scenes.push_back(std::move(scene));
    }
}

std::optional<uint> ParticleSystem::getParticleTypeIndex(std::string_view type_name) const {
    for(uint i = 0; i < m_particle_types.size(); ++i) {
        if(m_particle_types[i].name == type_name) return i;
    }
    return {};
}

bool ParticleSystem::configure(xml::Element const* properties) {
    if(!properties) return false;

    for(auto particle_type_props : xml::IterateChildElements(properties, "ParticleType")) {
        std::string name;
        if(!xml::getAttribute(particle_type_props, "name", name)) {
            logError("ParticleType is missing 'name' Attribute");
            return false;
        }
        float max_age;
        if(!xml::getAttribute(particle_type_props, "max_age", max_age)) {
            logError("ParticleType is missing 'max_age' Attribute");
            return false;
        }

        vec3                 acceleration = {0, 0, 0};
        float                drag         = {0};
        std::array<float, 3> size         = {1, 0, 0};
        xml::getAttribute(particle_type_props, "acceleration", acceleration);
        xml::getAttribute(particle_type_props, "drag", drag);
        xml::getAttribute(particle_type_props, "size_0", size[0]);
        xml::getAttribute(particle_type_props, "size_1", size[1]);
        xml::getAttribute(particle_type_props, "size_2", size[2]);

        std::vector<vec4> tex_coords;
        for(auto frame : xml::IterateChildElements(particle_type_props, "Frame")) {
            vec4 tc = {0, 0, 1, 1};
            xml::getAttribute(frame, "tex_coords", tc);
            tex_coords.push_back(tc);
        }

        m_particle_types.emplace_back(
            name, max_age, acceleration, drag, size, std::move(tex_coords)
        );
    }
    return true;
}

bool ParticleSystem::init() {
    return true;
}

void ParticleSystem::shutdown() {}

void ParticleSystem::update(double time_since_last_update) {
    for(auto& it : m_scenes) {
        auto scene = it.lock();
        if(!scene) continue;
        auto particles = scene->getDataContainer<ParticleData>();
        particles->update(time_since_last_update);

        auto* transform_container = scene->getComponentContainer<Transform>();
        auto* emitter_container   = scene->getComponentContainer<ParticleEmitter>();

        ComponentMask transform_mask = transform_container->getComponentMask();
        ComponentMask emitter_mask   = emitter_container->getComponentMask();

        auto& component_masks = scene->getEntitys();
        auto& emitters        = emitter_container->getComponentData();
        for(auto& [entity, emitter] : emitters) {
            vec3  position = {0.f, 0.f, 0.f};
            quat  rotation = {1.f, 0.f, 0.f, 0.f};
            float scale    = 1.f;
            if(!(component_masks[std::to_underlying(entity)] & emitter_mask)) continue;
            if(component_masks[std::to_underlying(entity)] & transform_mask) {
                auto* transform = transform_container->getComponent(entity);
                position        = transform->position;
                rotation        = transform->rotation;
                scale           = transform->scale;
            }

            for(auto& it : emitter.effects) {
                auto effect = it.effect.get();
                if(!effect) continue;

                it.cooldown -= time_since_last_update;
                if(it.cooldown <= 0) {
                    it.cooldown += it.min_cooldown;
                    particles->spawnParticleEffect(*effect, position, rotation, scale);
                }
            }
        }
    }
}

} // namespace bembel::particles
