#include "./environment-map-reflection-stage.hpp"

#include "./camera.hpp"
#include "./rendering-pipeline.hpp"

namespace bembel::graphics {

EnvironmentMapReflectionStage::EnvironmentMapReflectionStage(RenderingPipeline& pipline)
: RenderingStage(pipline) {
}

EnvironmentMapReflectionStage::~EnvironmentMapReflectionStage() {
}

namespace {
    bool loadCubeMapSide(GLenum target, const std::string& file) {
        base::Image image;
        if(!image.load(file, false)) return false;

        glTexImage2D(
            target,
            0,
            static_cast<GLint>(GL_RGB8),
            image.getWidth(),
            image.getHeight(),
            0,
            image.getChannels() == 4 ? GL_RGBA : GL_RGB,
            GL_UNSIGNED_BYTE,
            image.getData());
        return true;
    }
} // namespace

bool EnvironmentMapReflectionStage::initEnvironmentMap(
    const std::string& left,
    const std::string& right,
    const std::string& bottom,
    const std::string& top,
    const std::string& back,
    const std::string& front) {
    this->environment_map = std::make_unique<kernel::Texture>(GL_TEXTURE_CUBE_MAP, GL_RGBA);
    this->environment_map->bind();

    if(!loadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left)
       || !loadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right)
       || !loadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom)
       || !loadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top)
       || !loadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front)
       || !loadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back)) {
        this->environment_map.reset();
        return false;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GLint(GL_LINEAR));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GLint(GL_LINEAR_MIPMAP_LINEAR));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GLint(GL_CLAMP_TO_EDGE));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GLint(GL_CLAMP_TO_EDGE));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GLint(GL_CLAMP_TO_EDGE));
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    this->environment_map->release();

    return true;
}

void EnvironmentMapReflectionStage::setShader(kernel::Asset<kernel::ShaderProgram> shader) {
    this->shader_program = shader;
}

void EnvironmentMapReflectionStage::setOutputTexture(const std::string& texture) {
    this->fbo->setColorAttechment(0, this->pipline.getTexture(texture));
}

void EnvironmentMapReflectionStage::init() {
    this->fbo->init();
}

void EnvironmentMapReflectionStage::cleanup() {
    this->fbo->cleanup();
}

void EnvironmentMapReflectionStage::execute() {
    if(!shader_program) return;

    auto program = shader_program.get();
    if(!program) return;

    this->fbo->beginRenderToTexture();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    program->use();

    glm::mat4 proj    = this->pipline.getCamera()->getProjectionMatrix();
    glm::mat4 view    = this->pipline.getCamera()->getViewMatrix();
    glm::mat4 invProj = glm::inverse(proj);
    glm::mat4 invView = glm::inverse(view);

    glUniformMatrix4fv(
        program->getUniformLocation("uInverseProjectionMatrix"), 1, GL_FALSE, &invProj[0][0]);
    glUniformMatrix4fv(
        program->getUniformLocation("uInverseViewMatrix"), 1, GL_FALSE, &invView[0][0]);

    bindInputTextures();

    glActiveTexture(GL_TEXTURE0 + unsigned(this->textures.size()));
    this->environment_map->bind();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    this->environment_map->release();

    releaseInputTextures();

    glDisable(GL_BLEND);
    this->fbo->endRenderToTexture();
}

std::unique_ptr<EnvironmentMapReflectionStage> EnvironmentMapReflectionStage::createInstance(
    const base::xml::Element* properties, RenderingPipeline& pipline) {
    kernel::Asset<kernel::ShaderProgram> program;
    program.request(pipline.getAssetManager(), properties->FirstChildElement("Shader"));

    auto stage = std::make_unique<EnvironmentMapReflectionStage>(pipline);
    stage->setShader(program);

    const base::xml::Element* env_map = properties->FirstChildElement("EnvironmentMap");
    std::string               left, right, bottom, top, back, front;
    base::xml::getAttribute(env_map, "left", left);
    base::xml::getAttribute(env_map, "right", right);
    base::xml::getAttribute(env_map, "bottom", bottom);
    base::xml::getAttribute(env_map, "top", top);
    base::xml::getAttribute(env_map, "back", back);
    base::xml::getAttribute(env_map, "front", front);
    if(!stage->initEnvironmentMap(left, right, bottom, top, back, front)) return nullptr;

    std::string texture_name;
    if(base::xml::getAttribute(properties, "Output", "texture", texture_name))
        stage->setOutputTexture(texture_name);

    std::vector<std::string> intput_textures;
    for(auto input : base::xml::IterateChildElements(properties, "Input")) {
        if(base::xml::getAttribute(input, "texture", texture_name))
            intput_textures.push_back(texture_name);
    }
    stage->setInputTextures(intput_textures);

    return std::move(stage);
}

} // namespace bembel::graphics
