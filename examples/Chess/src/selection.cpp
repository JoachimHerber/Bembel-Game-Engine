module;
#include <glbinding/gl/gl.h>

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <random>
module bembel.examples.chess.selection;

import bembel;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;
using namespace ::gl;
using namespace std::chrono;

SelectionPointer::SelectionPointer(RenderingPipeline* pipline) : m_pipline(pipline) {
    events::addHandler<CursorMovedEvent>(this);
    events::addHandler<MouseButtonPressEvent>(this);
}

SelectionPointer::~SelectionPointer() {
    events::removeHandler<CursorMovedEvent>(this);
    events::removeHandler<MouseButtonPressEvent>(this);
}

void SelectionPointer::handleEvent(CursorMovedEvent const& event) {
    ivec2 windowSize = event.window->getWindowSize();
    ivec2 fbSize     = event.window->getFrameBufferSize();

    vec2 pos = event.position;

    // convert courser position form to window coordinates
    // to viewport coordinates.
    pos /= vec2(windowSize);
    pos.y = 1.0 - pos.y;
    pos *= fbSize;

    updateRay(pos);
}

void SelectionPointer::handleEvent(MouseButtonPressEvent const& event) {
    if(event.button_id == 0) m_select.emit();
}

void SelectionPointer::updateRay(vec2 pos) {
    const vec2 res = m_pipline->getResulution();

    for(auto const& it : m_pipline->getViews()) {
        auto vp = it->getViewport();
        if(!vp) continue;
        vec2 vp_pos = pos - vec2(vp->getPosition());
        vp_pos /= vec2(vp->getSize());

        // test if the cursor is within the view port
        if(!(0.0f <= vp_pos.x && vp_pos.x <= 1.0f && 0.0f <= vp_pos.y && vp_pos.y <= 1.0f))
            continue;

        vec2 tc = vec2(it->getViewAreaPosition()) + vec2(it->getViewAreaSize()) * vp_pos;
        tc /= res;

        auto cam = m_pipline->getCamera();

        vec4 rel_pos(2 * tc.x - 1, 2 * tc.y - 1, 0, 1);
        rel_pos = cam->getInverseProjectionMatrix() * rel_pos;
        rel_pos /= rel_pos.w;
        rel_pos = cam->getInverseViewMatrix() * rel_pos;

        m_ray_origin    = cam->getPosition();
        m_ray_direction = glm::vec3(rel_pos) - m_ray_origin;
        m_ray_direction = glm::normalize(m_ray_direction);
    }

    float     f = -m_ray_origin.y / m_ray_direction.y;
    glm::vec3 p = m_ray_origin + f * m_ray_direction;
    p *= 0.5f;

    m_selected_tile = glm::ivec2{p.x + 0.5f, p.z + 0.5f};
}

SelectionRenderingStage::SelectionRenderingStage(RenderingPipeline& pipline)
  : RenderingPipeline::Stage(pipline) {
    m_noise = std::make_unique<Texture>(Texture::Target::TEXTURE_3D, Texture::Format::R8);

    unsigned char                   data[32 * 32 * 32];
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);
    for(unsigned i = 0; i < 32 * 32 * 32; ++i) data[i] = distrib(gen);

    m_noise->init(Texture::MinFilter::LINEAR, Texture::MagFilter::LINEAR);
    m_noise->bind();

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, static_cast<GLint>(GL_REPEAT));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, static_cast<GLint>(GL_REPEAT));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, static_cast<GLint>(GL_REPEAT));
    glTexImage3D(
        GL_TEXTURE_3D, 0, static_cast<GLint>(GL_R8), 32, 32, 32, 0, GL_RED, GL_UNSIGNED_BYTE, data
    );

    m_noise->release();

    m_start_time = std::chrono::high_resolution_clock::now();
}

SelectionRenderingStage::~SelectionRenderingStage() {}

void SelectionRenderingStage::setShader(Asset<ShaderProgram> const& shader) {
    m_shader_program = shader;
}
void SelectionRenderingStage::setShader(Asset<ShaderProgram>&& shader) {
    m_shader_program = std::move(shader);
}

void SelectionRenderingStage::setDepthOutputTexture(std::string_view texture) {
    m_fbo->setDepthAttechment(m_pipline.getTexture(texture));
}

void SelectionRenderingStage::setColorOutputTexture(std::string_view texture) {
    m_fbo->setColorAttechment(0, m_pipline.getTexture(texture));
}

void SelectionRenderingStage::init() {
    m_fbo->init();
}

void SelectionRenderingStage::cleanup() {
    m_fbo->cleanup();
    m_shader_program.release();
}

void SelectionRenderingStage::execute(In<std::span<const RendererPtr>>) {
    if(!m_shader_program) return;

    m_fbo->beginRenderToTexture();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_3D);
    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader_program->use();
    m_noise->bind();

    auto         cam  = m_pipline.getCamera();
    glm::mat4    proj = cam->getProjectionMatrix();
    glm::mat4    view = cam->getViewMatrix();
    milliseconds ms   = duration_cast<milliseconds>(high_resolution_clock::now() - m_start_time);
    float        time = 0.001f * (ms.count());

    m_shader_program->setUniform("uNoise", 0);
    m_shader_program->setUniform("uProjectionMatrix", proj);
    m_shader_program->setUniform("uTime", time);

    std::vector<GeometryObject> geometry;

    getHiglightedObjects(geometry);

    std::sort(geometry.begin(), geometry.end(), [](auto& first, auto& second) {
        return first.dist > second.dist;
    });

    GeometryMesh* currentMesh = nullptr;
    for(auto& it : geometry) {
        glm::mat4 modelView{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

        modelView = view * glm::translate(glm::mat4(1), it.position) * glm::mat4_cast(it.rotation);

        m_shader_program->setUniform("uState", int(it.state));
        m_shader_program->setUniform("uModleViewMatrix", modelView);
        m_shader_program->setUniform("uNormalMatrix", modelView);

        auto mesh = it.model->getMesh();

        if(mesh == nullptr) continue;

        if(mesh != currentMesh) {
            currentMesh = mesh;
            glBindVertexArray(currentMesh->getVAO());
        }

        for(auto& mapping : it.model->getMateialMapping()) {
            auto mesh = currentMesh->getSubMesh(mapping.sub_mesh);
            if(mesh) {
                glDrawElements(
                    GL_TRIANGLES,
                    mesh.value().num_indices,
                    GL_UNSIGNED_INT,
                    (void*)(mesh.value().first_index * sizeof(unsigned))
                );
            }
        }
    }

    m_noise->release();
    m_fbo->endRenderToTexture();
}

void SelectionRenderingStage::setScene(Scene* scene) {
    m_scene = scene;
    if(m_scene) {
        m_transform_components = scene->getComponentContainer<Transform>();
        m_geometry_components  = scene->getComponentContainer<Geometry>();
        m_highlight_components = scene->getComponentContainer<SelectionHighlightComponent>();
    } else {
        m_transform_components = nullptr;
        m_geometry_components  = nullptr;
        m_highlight_components = nullptr;
    }
}

bool SelectionRenderingStage::configure(xml::Element const* properties) {
    Asset<ShaderProgram> program;
    program.request(properties->FirstChildElement("Shader"));

    setShader(std::move(program));

    std::string texture_name;
    if(xml::getAttribute(properties, "ColorOutput", "texture", texture_name))
        setColorOutputTexture(texture_name);
    if(xml::getAttribute(properties, "DepthOutput", "texture", texture_name))
        setDepthOutputTexture(texture_name);

    return true;
}

void SelectionRenderingStage::getHiglightedObjects(std::vector<GeometryObject>& objects) {
    glm::vec3 camPos = m_pipline.getCamera()->getPosition();

    auto const& entitis = m_scene->getEntitys();

    auto  transform  = m_transform_components->begin();
    auto& geometry   = m_geometry_components->getComponentData();
    auto& highlights = m_highlight_components->getComponentData();

    ComponentMask mask = m_transform_components->getComponentMask()
                       | m_geometry_components->getComponentMask()
                       | m_highlight_components->getComponentMask();

    for(usize entity = 0; entity < entitis.size(); ++entity, ++transform) {
        if((entitis[entity] & mask) != mask) continue;

        if(highlights[entity] == SelectionHighlight::NO_HIGHLIGHT) continue;

        GeometryModel* model = geometry[entity].model.get();
        if(model == nullptr) continue;

        float dist = glm::length(camPos - transform->position);

        objects.emplace_back(
            transform->position, transform->rotation, dist, model, unsigned(highlights[entity]) - 1
        );
    }
}

} // namespace bembel::examples::chess
