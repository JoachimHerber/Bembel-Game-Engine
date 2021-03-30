#include "./deferred-lighting-stage.hpp"

#include <bembel/base/logging/logger.hpp>

#include "./camera.hpp"
#include "./light-source.hpp"
#include "./rendering-pipeline.hpp"

namespace bembel::graphics {

DeferredLightingStage::DeferredLightingStage(RenderingPipeline& pipline)
: RenderingStage(pipline) {
}

DeferredLightingStage::~DeferredLightingStage() {
}

void DeferredLightingStage::setDirLightShader(kernel::Asset<kernel::ShaderProgram> shader) {
    this->dir_light_shader = shader;
}
void DeferredLightingStage::setPointLightShader(kernel::Asset<kernel::ShaderProgram> shader) {
    this->point_light_shader = shader;
}

void DeferredLightingStage::setScene(kernel::Scene* scene) {
    this->scene                 = scene;
    this->dir_light_container   = this->scene->requestComponentContainer<DirectionalLightSource>();
    this->point_light_container = this->scene->requestComponentContainer<PointLightSource>();
    this->position_container = this->scene->requestComponentContainer<kernel::PositionComponent>();
}

void DeferredLightingStage::init() {
    RenderingStage::init();

    this->buffer_size = 32;
    glGenBuffers(1, &(this->vbo));
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->buffer_size * 8 * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &(this->vao));
    glBindVertexArray(this->vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(0 * sizeof(float))); // position
    glVertexAttribPointer(
        1,
        1,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))); // bulbRadius
    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(4 * sizeof(float))); // color
    glVertexAttribPointer(
        3,
        1,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(7 * sizeof(float))); // cutoffRadius
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void DeferredLightingStage::cleanup() {
    RenderingStage::cleanup();
}

void DeferredLightingStage::execute() {
    if(!this->scene) return;

    this->fbo->beginRenderToTexture();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);

    bindInputTextures();

    applyDirectionalLights();
    applyPointLights();

    releaseInputTextures();

    glDisable(GL_BLEND);

    this->fbo->endRenderToTexture();
}

std::unique_ptr<DeferredLightingStage> DeferredLightingStage::createInstance(
    const base::xml::Element* properties, RenderingPipeline& pipline) {
    auto& asset_mgr = pipline.getAssetManager();

    kernel::Asset<kernel::ShaderProgram> dir_light_shader;
    kernel::Asset<kernel::ShaderProgram> point_light_shader;
    dir_light_shader.request(asset_mgr, properties->FirstChildElement("DirectionalLightProgram"));
    point_light_shader.request(asset_mgr, properties->FirstChildElement("PointLightShaderProgram"));

    auto stage = std::make_unique<DeferredLightingStage>(pipline);
    stage->setDirLightShader(dir_light_shader);
    stage->setPointLightShader(point_light_shader);

    std::string texture_name;
    if(base::xml::getAttribute(properties, "Output", "texture", texture_name))
        stage->setColorOutputTexture(0, texture_name);

    std::vector<std::string> intput_textures;
    for(auto input : base::xml::IterateChildElements(properties, "Input")) {
        if(base::xml::getAttribute(input, "texture", texture_name))
            intput_textures.push_back(texture_name);
    }
    stage->setInputTextures(intput_textures);

    return std::move(stage);
}

void DeferredLightingStage::applyDirectionalLights() {
    kernel::Scene::ComponentMask mask = this->dir_light_container->getComponentMask();

    auto shader = this->dir_light_shader.get();
    if(!shader) return;

    shader->use();
    const auto camera = this->pipline.getCamera();

    glm::mat4 inv_projection = camera->getInverseProjectionMatrix();

    glUniformMatrix4fv(
        shader->getUniformLocation("uInverseProjectionMatrix"), 1, GL_FALSE, &inv_projection[0][0]);

    glm::mat3 normal_matrix = camera->getViewMatrix();

    for(const auto& it : this->dir_light_container->getComponents()) {
        glm::vec3 dir = it.second.direction;
        dir           = normal_matrix * dir;

        glUniform3f(
            shader->getUniformLocation("uLigthColor"),
            it.second.color.r,
            it.second.color.g,
            it.second.color.b);
        glUniform3f(shader->getUniformLocation("uLigthDir"), dir.x, dir.y, dir.z);

        if((this->scene->getEntitys()[it.first] & mask) != mask) continue; // this should not happen

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glUseProgram(0);
}

struct PointLight {
    float x, y, z;
    float bulb_radius;
    float r, g, b;
    float cutoff_radius;
};

void DeferredLightingStage::applyPointLights() {
    auto shader = this->point_light_shader.get();
    if(!shader) return;

    kernel::Scene::ComponentMask mask = this->point_light_container->getComponentMask()
                                        | this->position_container->getComponentMask();

    Camera* camera = this->pipline.getCamera().get();

    std::vector<PointLight> point_lights;
    for(const auto& it : this->point_light_container->getComponents()) {
        if((this->scene->getEntitys()[it.first] & mask) != mask) continue; // this should not happen

        PointLight light;

        glm::vec4 position =
            glm::vec4(this->position_container->getComponent(it.first)->position, 1);
        position = camera->getViewMatrix() * position;

        light.x = position.x;
        light.y = position.y;
        light.z = position.z;

        light.bulb_radius = it.second.bulb_radius;

        light.r = it.second.color.r;
        light.g = it.second.color.g;
        light.b = it.second.color.b;

        light.cutoff_radius = it.second.cutoff_radius;
        point_lights.push_back(light);
    }

    shader->use();

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    glm::mat4 projection        = camera->getProjectionMatrix();
    glm::mat4 invers_projection = camera->getInverseProjectionMatrix();

    glUniformMatrix4fv(
        shader->getUniformLocation("uProjectionMatrix"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(
        shader->getUniformLocation("uInverseProjectionMatrix"),
        1,
        GL_FALSE,
        &invers_projection[0][0]);
    glUniform2f(
        shader->getUniformLocation("uTexelSize"),
        1.0f / this->pipline.getResulution().x,
        1.0f / this->pipline.getResulution().y);

    for(size_t n = 0; n < point_lights.size(); n += this->buffer_size) {
        GLsizei num_lights = std::min(this->buffer_size, unsigned(point_lights.size() - n));
        glBufferSubData(
            GL_ARRAY_BUFFER, 0, num_lights * sizeof(PointLightSource), &point_lights[n]);

        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 10, num_lights);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

} // namespace bembel::graphics
