module;
#include <glbinding/gl/gl.h>

#include <memory>
module bembel.graphics.pipeline;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace ::gl;

Factory<RenderingPipeline::Stage, RenderingPipeline&>& RenderingPipeline::Stage::getFactory() {
    static Factory<Stage, RenderingPipeline&> factory;
    return factory;
}

void RenderingPipeline::Stage::setInputTextures(std::span<std::string> textures) {
    m_textures.clear();
    for(size_t n = 0; n < textures.size(); ++n) {
        m_textures.push_back(m_pipline.getTexture(textures[n]));
    }
}

void RenderingPipeline::Stage::setDepthOutputTexture(std::string_view texture) {
    m_fbo->setDepthAttechment(m_pipline.getTexture(texture));
}

void RenderingPipeline::Stage::setColorOutputTexture(unsigned index, std::string_view texture) {
    m_fbo->setColorAttechment(index, m_pipline.getTexture(texture));
}

void RenderingPipeline::Stage::bindInputTextures() {
    for(unsigned n = 0; n < m_textures.size(); ++n) {
        if(m_textures[n]) {
            glActiveTexture(GL_TEXTURE0 + n);
            m_textures[n]->bind();
        }
    }
}

void RenderingPipeline::Stage::releaseInputTextures() {
    for(uint n = uint(m_textures.size()); n-- > 0;) {
        if(m_textures[n]) {
            glActiveTexture(GL_TEXTURE0 + n);
            m_textures[n]->release();
        }
    }
}

void RenderingPipeline::View::draw(In<ivec2> viewport_position, In<uvec2> viewport_size) {
    m_fbo->blitToBackBuffer(
        m_view_area_pos,
        m_view_area_pos + glm::ivec2(m_view_area_size),
        viewport_position,
        viewport_position + glm::ivec2(viewport_size)
    );
}

void RenderingPipeline::setResulution(In<ivec2> value) {
    if(m_resolution == value) return;

    m_resolution = value;
    if(m_initalized) {
        for(auto& [name, texture] : m_textures) texture->setSize(value);
    }

    for(auto& view : m_views) view->updateViewArea(m_resolution);
}

bool RenderingPipeline::configure(xml::Element const* properties) {
    if(!properties) return false;

    xml::getAttribute(properties, "horizontal_resolution", m_resolution.x);
    xml::getAttribute(properties, "vertical_resolution", m_resolution.y);

    configureTextures(properties->FirstChildElement("Textures"));
    configureStages(properties->FirstChildElement("RenderingStages"));
    configureViews(properties->FirstChildElement("Views"));
    configureCamera(properties->FirstChildElement("Camera"));
    return true;
}

void RenderingPipeline::init() {
    for(auto& [name, texture] : m_textures)
        texture->init(m_resolution, Texture::MinFilter::LINEAR, Texture::MagFilter::LINEAR);

    for(auto& stage : m_render_stages) stage->init();

    for(auto& view : m_views) view->init();

    m_initalized = true;
}

void RenderingPipeline::cleanup() {
    for(auto& view : m_views) view->cleanup();

    for(auto& stage : m_render_stages) stage->cleanup();

    for(auto& it : m_textures) it.second->cleanup();

    m_initalized = false;
}

void RenderingPipeline::setScene(std::shared_ptr<Scene> scene) {
    m_scene = scene;

    for(auto& stage : m_render_stages) stage->setScene(m_scene.get());
}

Texture* RenderingPipeline::getTexture(std::string_view name) const {
    auto it = m_textures.find(name);
    if(it != m_textures.end()) return it->second.get();

    return nullptr;
}

bool RenderingPipeline::createTexture(std::string_view name, Texture::Format format) {
    if(m_textures.find(name) != m_textures.end()) return false; // texture already exists

    auto texture = std::make_unique<Texture>(Texture::Target::TEXTURE_2D, format);
    if(m_initalized)
        texture->init(m_resolution, Texture::MinFilter::LINEAR, Texture::MagFilter::LINEAR);

    m_textures.emplace(name, std::move(texture));
    return true;
}

bool RenderingPipeline::createTexture(xml::Element const* properties) {
    std::string name, format;
    if(!xml::getAttribute(properties, "name", name)) return false;

    xml::getAttribute(properties, "format", format);

    auto f = Texture::stringToTextureFormat(format);
    if(f.has_value()) {
        return createTexture(name, *f);
    } else {
        return false;
    }
}

RenderingPipeline::View* RenderingPipeline::createView(std::string_view texture_name) {
    Texture* texture = getTexture(texture_name);
    if(!texture) return nullptr;

    m_views.push_back(std::make_unique<View>(texture));
    if(m_initalized) m_views.back()->init();

    return m_views.back().get();
}

void RenderingPipeline::update(In<std::span<const RendererPtr>> renderer) {
    gl::glViewport(0, 0, m_resolution.x, m_resolution.y);
    for(auto& stage : m_render_stages) { stage->execute(renderer); }
}

void RenderingPipeline::configureTextures(xml::Element const* properties) {
    if(!properties) return;

    for(auto textureProperties : xml::IterateChildElements(properties, "Texture")) {
        createTexture(textureProperties);
    }
}

void RenderingPipeline::configureStages(xml::Element const* properties) {
    if(!properties) return;

    for(auto stageProperties : xml::IterateChildElements(properties)) {
        auto stage = Stage::create(stageProperties->Value(), *this);

        if(stage && stage->configure(stageProperties)) m_render_stages.push_back(std::move(stage));
    }
}

void RenderingPipeline::configureViews(xml::Element const* properties) {
    if(!properties) return;

    for(auto viewProperties : xml::IterateChildElements(properties, "View")) {
        std::string texture;
        xml::getAttribute(viewProperties, "texture", texture);

        auto  view = createView(texture);
        float l = 0, r = 1, b = 0, t = 1;
        xml::getAttribute(viewProperties, "left", l);
        xml::getAttribute(viewProperties, "right", r);
        xml::getAttribute(viewProperties, "bottom", b);
        xml::getAttribute(viewProperties, "top", t);
        view->setRelativeViewArea(vec2(l, b), vec2(r - l, t - b));
        view->updateViewArea(m_resolution);

        unsigned windowId, viewportId;
        if(xml::getAttribute(viewProperties, "window", windowId)
           && xml::getAttribute(viewProperties, "viewport", viewportId)) {
            auto window = m_display_mgr.getWindow(windowId);

            if(window && window->getViewports().size() > viewportId) {
                window->getViewports()[viewportId]->addView(view);
            }
        }
    }
}

void RenderingPipeline::configureCamera(xml::Element const* properties) {
    if(!properties) return;

    vec3 pos;
    xml::getAttribute(properties, "Position", "x", pos.x);
    xml::getAttribute(properties, "Position", "y", pos.y);
    xml::getAttribute(properties, "Position", "z", pos.z);
    m_camera->setPosition(pos);

    float fov = 60, ar = (4 / 3), near = 0.1f, far = 1000.0f;
    xml::getAttribute(properties, "Projection", "fild_of_view", fov);
    xml::getAttribute(properties, "Projection", "aspect_ration", ar);
    xml::getAttribute(properties, "Projection", "near", near);
    xml::getAttribute(properties, "Projection", "far", far);
    fov *= 3.14159265359f / 180.0f;
    m_camera->setUpProjection(fov, ar, near, far);
}

} // namespace bembel::graphics
