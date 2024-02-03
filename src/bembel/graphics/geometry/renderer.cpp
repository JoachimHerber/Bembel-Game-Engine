module;
#include <glbinding/gl/gl.h>

#include <memory>
#include <utility>
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace ::gl;

void DefaultGeometryRenderer::renderGeometry(
    In<mat4> proj, In<mat4> view, In<std::vector<GeometryRenderData>> data
) {
    if(!m_geomety_pass_shader) return;
    initDummyTextures();

    auto bindTexture = [](Asset<Texture> const& texture, std::unique_ptr<Texture> const& fallback) {
        if(auto t = texture.get())
            t->bind();
        else
            fallback->bind();
    };

    glCullFace(GL_BACK);

    m_geomety_pass_shader->use();
    m_geomety_pass_shader->setUniform("uProjectionMatrix", proj);

    GeometryMesh* currentMesh     = nullptr;
    Material*     currentMaterial = nullptr;
    for(auto const& it : data) {
        if(!it.mesh || !it.material) continue;

        if(it.mesh != currentMesh) {
            currentMesh = it.mesh;
            glBindVertexArray(currentMesh->getVAO());
        }
        if(it.material != currentMaterial) {
            currentMaterial = it.material;
            currentMaterial->bindUniformBufferObject();

            glActiveTexture(GL_TEXTURE0);
            bindTexture(currentMaterial->getEmissionTexture(), m_dummy_emission_texture);
            glActiveTexture(GL_TEXTURE1);
            bindTexture(currentMaterial->getBaseColorTexture(), m_dummy_base_color_texture);
            glActiveTexture(GL_TEXTURE2);
            bindTexture(currentMaterial->getMaterialPropsTexture(), m_dummy_material_props_texture);
            glActiveTexture(GL_TEXTURE3);
            bindTexture(currentMaterial->getNormalMapTexture(), m_dummy_normal_map_texture);
        }

        mat4 modleView = view * it.transform;

        m_geomety_pass_shader->setUniform("uModleViewMatrix", modleView);
        m_geomety_pass_shader->setUniform("uNormalMatrix", modleView);

        glDrawElements(
            GL_TRIANGLES,
            it.sub_mesh.num_indices,
            GL_UNSIGNED_INT,
            (void*)(it.sub_mesh.first_index * sizeof(unsigned))
        );
    }

    glActiveTexture(GL_TEXTURE0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void DefaultGeometryRenderer::renderShadows(
    In<mat4> view_proj, In<std::vector<GeometryRenderData>> data
) {
    if(!m_depth_pass_shader) return;
    glCullFace(GL_FRONT);

    m_depth_pass_shader->use();

    m_depth_pass_shader->setUniform("uProjectionMatrix", view_proj);

    GeometryMesh* currentMesh = nullptr;
    for(auto const& it : data) {
        if(!it.mesh) continue;
        if(it.mesh->getVertexFormat() != m_required_vertex_attributes) continue;

        if(it.mesh != currentMesh) {
            currentMesh = it.mesh;
            glBindVertexArray(currentMesh->getVAO());
        }
        glUniformMatrix4fv(
            m_depth_pass_shader->getUniformLocation("uModleViewMatrix"),
            1,
            GL_FALSE,
            &(it.transform[0][0])
        );
        glDrawElements(
            GL_TRIANGLES,
            it.sub_mesh.num_indices,
            GL_UNSIGNED_INT,
            (void*)(it.sub_mesh.first_index * sizeof(unsigned))
        );
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

std::unique_ptr<DefaultGeometryRenderer> DefaultGeometryRenderer::createRenderer(
    xml::Element const* properties
) {
    Asset<ShaderProgram> geomety_pass_shader;
    if(!geomety_pass_shader.request(properties->FirstChildElement("GeometyPassShader"))) {
        logError("Could not load ShaderProgram for geomety pass of DefaultGeometryRenderer");
        return nullptr;
    }
    Asset<ShaderProgram> shadow_pass_shader;
    if(!shadow_pass_shader.request(properties->FirstChildElement("ShadowPassShader"))) {
        logError("Could not load ShaderProgram for shadow pass of DefaultGeometryRenderer");
        return nullptr;
    }

    GLint count;
    glGetProgramiv(geomety_pass_shader->getHandl(), GL_ACTIVE_ATTRIBUTES, &count);

    VertexAttribMask mask{0};
    for(GLint i = 0; i < count; i++) {
        constexpr GLsizei bufSize = 256;
        GLchar            name[bufSize];
        GLsizei           length;
        GLint             size;
        GLenum            type;

        glGetActiveAttrib(
            geomety_pass_shader->getHandl(), (GLuint)i, bufSize, &length, &size, &type, name
        );
        auto loc = glGetAttribLocation(geomety_pass_shader->getHandl(), name);
        if(loc >= 0) mask |= VertexAttribMask(1 << loc);
    }

    auto renderer = std::make_unique<DefaultGeometryRenderer>(mask);

    if(!renderer->setShaders(std::move(geomety_pass_shader), std::move(shadow_pass_shader)))
        return nullptr;

    return std::move(renderer);
}

void DefaultGeometryRenderer::initDummyTextures() {
    auto initDummyTexture = [](In<vec3> color) {
        auto texture =
            std::make_unique<Texture>(Texture::Target::TEXTURE_2D, Texture::Format::RGB8);
        texture->init(
            Texture::MinFilter::NEAREST,
            Texture::MagFilter::NEAREST,
            Texture::Wrap::CLAMP_TO_EDGE,
            Texture::Wrap::CLAMP_TO_EDGE
        );
        texture->bind();
        glTexImage2D(
            GL_TEXTURE_2D, 0, static_cast<GLint>(GL_RGB8), 1, 1, 0, GL_RGB, GL_FLOAT, &color
        );
        texture->release();
        return texture;
    };

    if(!m_dummy_emission_texture) //
        m_dummy_emission_texture = initDummyTexture({1.f, 1.f, 1.f});

    if(!m_dummy_base_color_texture) //
        m_dummy_base_color_texture = initDummyTexture({1.f, 1.f, 1.f});

    if(!m_dummy_material_props_texture)
        m_dummy_material_props_texture = initDummyTexture({1.f, 1.f, 1.f});

    if(!m_dummy_normal_map_texture)
        m_dummy_normal_map_texture = initDummyTexture({0.5f, 0.5f, 1.f});
}

} // namespace bembel::graphics
