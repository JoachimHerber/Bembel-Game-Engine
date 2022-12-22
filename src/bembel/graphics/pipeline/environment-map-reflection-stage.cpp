module;
#include <glbinding/gl/gl.h>

#include <glm/gtc/matrix_inverse.hpp>
#include <utility>
module bembel.graphics.pipeline;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace ::gl;

using GLenum = gl::GLenum;

EnvironmentMapReflectionStage::EnvironmentMapReflectionStage(RenderingPipeline& pipline)
  : RenderingPipeline::Stage(pipline) {}

EnvironmentMapReflectionStage::~EnvironmentMapReflectionStage() {}

namespace {
    bool loadCubeMapSide(GLenum target, std::string_view file) {
        Image image;
        if(!image.load(file, false)) return false;

        glTexImage2D(
            target,
            0,
            GL_RGB8,
            image.getWidth(),
            image.getHeight(),
            0,
            image.getChannels() == 4 ? GL_RGBA : GL_RGB,
            GL_UNSIGNED_BYTE,
            image.getData()
        );
        return true;
    }
} // namespace

bool EnvironmentMapReflectionStage::initEnvironmentMap(
    std::string_view left,
    std::string_view right,
    std::string_view bottom,
    std::string_view top,
    std::string_view back,
    std::string_view front
) {
    m_environment_map =
        std::make_unique<kernel::Texture>(Texture::Target::CUBE_MAP, Texture::Format::RGBA);
    m_environment_map->bind();

    if(!loadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left)
       || !loadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right)
       || !loadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom)
       || !loadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top)
       || !loadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front)
       || !loadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back)) {
        m_environment_map.reset();
        return false;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GLint(GL_LINEAR));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GLint(GL_LINEAR_MIPMAP_LINEAR));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GLint(GL_CLAMP_TO_EDGE));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GLint(GL_CLAMP_TO_EDGE));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GLint(GL_CLAMP_TO_EDGE));
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    m_environment_map->release();

    return true;
}

void EnvironmentMapReflectionStage::setShader(Asset<ShaderProgram> shader) {
    m_shader_program = std::move(shader);
}

void EnvironmentMapReflectionStage::setOutputTexture(std::string_view texture) {
    m_fbo->setColorAttechment(0, m_pipline.getTexture(texture));
}

void EnvironmentMapReflectionStage::init() {
    m_fbo->init();
}

void EnvironmentMapReflectionStage::cleanup() {
    m_fbo->cleanup();
}

void EnvironmentMapReflectionStage::execute(GeometryRenderQueue&, std::vector<RendererPtr> const&) {
    if(!m_shader_program) return;

    auto program = m_shader_program.getAsset();
    if(!program) return;

    m_fbo->beginRenderToTexture();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    program->use();

    mat4 proj    = m_pipline.getCamera()->getProjectionMatrix();
    mat4 view    = m_pipline.getCamera()->getViewMatrix();
    mat4 invProj = glm::inverse(proj);
    mat4 invView = glm::inverse(view);

    glUniformMatrix4fv(
        program->getUniformLocation("uInverseProjectionMatrix"), 1, GL_FALSE, &invProj[0][0]
    );
    glUniformMatrix4fv(
        program->getUniformLocation("uInverseViewMatrix"), 1, GL_FALSE, &invView[0][0]
    );

    bindInputTextures();

    glActiveTexture(GL_TEXTURE0 + unsigned(m_textures.size()));
    m_environment_map->bind();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_environment_map->release();

    releaseInputTextures();

    glDisable(GL_BLEND);
    m_fbo->endRenderToTexture();
}

bool EnvironmentMapReflectionStage::configure(xml::Element const* properties) {
    kernel::Asset<kernel::ShaderProgram> program;
    program.request(m_pipline.getAssetManager(), properties->FirstChildElement("Shader"));

    setShader(std::move(program));

    xml::Element const* env_map = properties->FirstChildElement("EnvironmentMap");
    std::string         left, right, bottom, top, back, front;
    xml::getAttribute(env_map, "left", left);
    xml::getAttribute(env_map, "right", right);
    xml::getAttribute(env_map, "bottom", bottom);
    xml::getAttribute(env_map, "top", top);
    xml::getAttribute(env_map, "back", back);
    xml::getAttribute(env_map, "front", front);
    if(!initEnvironmentMap(left, right, bottom, top, back, front)) return false;

    std::string texture_name;
    if(xml::getAttribute(properties, "Output", "texture", texture_name))
        setOutputTexture(texture_name);

    std::vector<std::string> intput_textures;
    for(auto input : xml::IterateChildElements(properties, "Input")) {
        if(xml::getAttribute(input, "texture", texture_name))
            intput_textures.push_back(texture_name);
    }
    setInputTextures(intput_textures);

    return true;
}

} // namespace bembel::graphics
