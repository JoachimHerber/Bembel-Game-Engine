#include "selection-rendering-stage.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

#include "selection-component.h"

using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;

SelectionRenderingStage::SelectionRenderingStage(RenderingPipeline& pipline)
: RenderingStage(pipline) {
    this->noise = std::make_unique<Texture>(GL_TEXTURE_3D, GL_R8);

    unsigned char                   data[32 * 32 * 32];
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dist(0, 255);
    for(unsigned i = 0; i < 32 * 32 * 32; ++i) data[i] = dist(gen);

    this->noise->init(GL_LINEAR, GL_LINEAR);
    this->noise->bind();

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, static_cast<GLint>(GL_REPEAT));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, static_cast<GLint>(GL_REPEAT));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, static_cast<GLint>(GL_REPEAT));
    glTexImage3D(
        GL_TEXTURE_3D, 0, static_cast<GLint>(GL_R8), 32, 32, 32, 0, GL_RED, GL_UNSIGNED_BYTE, data);

    this->noise->release();

    this->start_time = std::chrono::high_resolution_clock::now();
}

SelectionRenderingStage::~SelectionRenderingStage() {
}

void SelectionRenderingStage::setShader(const Asset<ShaderProgram>& shader) {
    this->shader_program = shader;
}
void SelectionRenderingStage::setShader(Asset<ShaderProgram>&& shader) {
    this->shader_program = std::move(shader);
}

void SelectionRenderingStage::setDepthOutputTexture(const std::string& texture) {
    this->fbo->setDepthAttechment(this->pipline.getTexture(texture));
}
void SelectionRenderingStage::setColorOutputTexture(const std::string& texture) {
    this->fbo->setColorAttechment(0, this->pipline.getTexture(texture));
}
void SelectionRenderingStage::init() {
    this->fbo->init();
}

void SelectionRenderingStage::cleanup() {
    this->fbo->cleanup();
    this->shader_program.release();
}

void SelectionRenderingStage::execute() {
    auto program = this->shader_program.get();
    if(!program) return;

    this->fbo->beginRenderToTexture();
    glm::mat4 proj = this->pipline.getCamera()->getProjectionMatrix();
    glm::mat4 view = this->pipline.getCamera()->getViewMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_3D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    program->use();
    this->noise->bind();

    glUniform1i(program->getUniformLocation("uNoise"), 0);
    glUniformMatrix4fv(program->getUniformLocation("uProjectionMatrix"), 1, GL_FALSE, &proj[0][0]);

    auto                      now = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - this->start_time);

    float time = 0.001f * (ms.count());

    glUniform1f(program->getUniformLocation("uTime"), time);

    std::vector<GeometryObject> geometry;

    this->getHiglightedObjects(geometry);

    std::sort(geometry.begin(), geometry.end(), [](auto& first, auto& second) {
        return first.dist > second.dist;
    });

    GeometryMesh* currentMesh = nullptr;
    for(auto& it : geometry) {
        glm::mat4 modelView =
            view * glm::translate(glm::mat4(), it.position) * glm::mat4_cast(it.rotation);

        glUniform1i(program->getUniformLocation("uState"), it.state);

        glUniformMatrix4fv(
            program->getUniformLocation("uModleViewMatrix"), 1, GL_FALSE, &modelView[0][0]);
        glUniformMatrix4fv(
            program->getUniformLocation("uNormalMatrix"), 1, GL_FALSE, &modelView[0][0]);

        auto mesh = it.model->getMesh();

        if(mesh == nullptr) continue;

        if(mesh != currentMesh) {
            currentMesh = mesh;
            glBindVertexArray(currentMesh->getVAO());
        }

        for(auto& mapping : it.model->getMateialMapping()) {
            unsigned first, count;
            if(currentMesh->getSubMesh(mapping.sub_mesh, first, count)) {
                glDrawElements(
                    GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(first * sizeof(unsigned)));
            }
        }
    }

    this->noise->release();
    this->fbo->endRenderToTexture();
    glDisable(GL_BLEND);
}

void SelectionRenderingStage::setScene(Scene* scene) {
    this->scene                = scene;
    this->position_components  = scene->requestComponentContainer<PositionComponent>();
    this->rotation_components  = scene->requestComponentContainer<RotationComponent>();
    this->selection_components = scene->requestComponentContainer<SelectionComponent>();
    this->geometry_components  = scene->requestComponentContainer<GeometryComponent>();
}

std::unique_ptr<SelectionRenderingStage> SelectionRenderingStage::createInstance(
    const xml::Element* properties, RenderingPipeline& pipline) {
    auto& asset_mgr = pipline.getAssetManager();

    Asset<ShaderProgram> program;
    program.request(asset_mgr, properties->FirstChildElement("Shader"));

    auto stage = std::make_unique<SelectionRenderingStage>(pipline);

    stage->setShader(std::move(program));

    std::string texture_name;
    if(xml::getAttribute(properties, "ColorOutput", "texture", texture_name))
        stage->setColorOutputTexture(texture_name);
    if(xml::getAttribute(properties, "DepthOutput", "texture", texture_name))
        stage->setDepthOutputTexture(texture_name);

    return std::move(stage);
}

void SelectionRenderingStage::getHiglightedObjects(std::vector<GeometryObject>& objects) {
    glm::vec3 camPos = this->pipline.getCamera()->getPosition();

    const auto& entitis = this->scene->getEntitys();

    auto& positions = this->position_components->getComponents();
    auto& rotations = this->rotation_components->getComponents();
    auto& selection = this->selection_components->getComponents();
    auto& geometry  = this->geometry_components->getComponents();

    Scene::ComponentMask mask = this->position_components->getComponentMask()
                                | this->selection_components->getComponentMask()
                                | this->geometry_components->getComponentMask();

    for(Scene::EntityID entity = 0; entity < entitis.size(); ++entity) {
        if((entitis[entity] & mask) != mask) continue;

        if(selection[entity].state == SelectionComponent::State::UNSELECTABLE) continue;

        auto model = this->scene->getAsset<GeometryModel>(geometry[entity].model);

        if(model == nullptr) continue;

        float dist = glm::length(camPos - positions[entity].position);

        glm::quat roatation;
        if(entitis[entity] & this->rotation_components->getComponentMask())
            roatation = rotations[entity].rotation;

        objects.push_back(
            {positions[entity].position,
             roatation,
             dist,
             model,
             unsigned(selection[entity].state) - 1u});
    }
}
