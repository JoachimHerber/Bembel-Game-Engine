module;
#include <glbinding/gl/gl.h>

#include "bembel/pch.h"
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace ::gl;

bool DefaultGeometryRenderer::updateUniformLocations(ShaderProgram* shader) {
    if(shader == nullptr) return false;

    m_material_uniform_block_index = shader->getUniformBlockIndex("Material");
    m_material_uniform_buffer_size =
        shader->getUniformBlockDataSize(m_material_uniform_block_index);

    std::vector<GLint> active_uniform_indices;
    shader->getUniformBlockActiveUniformIndices(
        m_material_uniform_block_index, &active_uniform_indices
    );

    for(GLint uniform_index : active_uniform_indices) {
        MaterialParam param;
        param.offset = shader->getActiveUniformOffset(uniform_index);
        std::string name;
        shader->getActiveUniform(uniform_index, &param.size, &param.type, &name);

        m_material_params[name]       = param;
        std::string param_name        = name.substr(name.find_last_of(".") + 1);
        m_material_params[param_name] = param;
    }

    shader->use();
    uint sampler_location = 0;
    for(auto const& it : m_required_textures) {
        glUniform1i(shader->getUniformLocation(it.uniform_sampler_name), sampler_location);
        ++sampler_location;
    }
    return true;
}

void DefaultGeometryRenderer::render(
    mat4 const& proj, mat4 const& view, std::vector<GeometryRenderData> const& data
) {
    auto shader_pointer = m_shader.getAsset();

    if(!shader_pointer) return;

    shader_pointer->use();

    glUniformMatrix4fv(
        shader_pointer->getUniformLocation("uProjectionMatrix"), 1, GL_FALSE, &proj[0][0]
    );

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
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, currentMaterial->getUniformBufferObject());

            GLenum active_texture = GL_TEXTURE0;
            for(auto const& texture : currentMaterial->getTextures()) {
                if(!texture) {
                    log().warning("Texture handle is invalid");
                    continue;
                }
                glActiveTexture(active_texture);
                texture->bind();
                active_texture = active_texture + 1;
            }
        }

        glm::mat4 modleView = view * it.transform;

        glUniformMatrix4fv(
            shader_pointer->getUniformLocation("uModleViewMatrix"), 1, GL_FALSE, &(modleView[0][0])
        );
        glUniformMatrix4fv(
            shader_pointer->getUniformLocation("uNormalMatrix"), 1, GL_FALSE, &(modleView[0][0])
        );

        glLoadIdentity();
        glMultMatrixf(&(it.transform[0][0]));
        glDrawElements(
            GL_TRIANGLES, it.count, GL_UNSIGNED_INT, (void*)(it.first * sizeof(unsigned))
        );
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

template <typename T>
bool ReadMaterialParam(xml::Element const* properties, std::string_view param_name, byte* dest) {
    T value;
    if(xml::getAttribute(properties, param_name, value)) {
        memcpy(dest, &value, sizeof(value));
        return true;
    }
    return false;
}

bool DefaultGeometryRenderer::readMaterialParameter(
    xml::Element const*  properties,
    std::string_view     param_name,
    MaterialParam const& param,
    byte*                buffer
) {
    switch(param.type) {
        case GL_FLOAT:
            return ReadMaterialParam<float>(properties, param_name, buffer + param.offset);
        case GL_FLOAT_VEC2:
            return ReadMaterialParam<glm::vec2>(properties, param_name, buffer + param.offset);
        case GL_FLOAT_VEC3:
            return ReadMaterialParam<glm::vec3>(properties, param_name, buffer + param.offset);
        case GL_FLOAT_VEC4:
            return ReadMaterialParam<glm::vec4>(properties, param_name, buffer + param.offset);
    }
    return false;
}

std::unique_ptr<Material> DefaultGeometryRenderer::createMaterial(xml::Element const* properties) {
    auto mat = std::make_unique<Material>(getRendererID(), m_material_uniform_buffer_size);

    std::vector<Asset<Texture>> textures;
    for(auto& it : m_required_textures) {
        auto texture_name = properties->FirstChildElement(it.texture_name.c_str());
        if(texture_name == nullptr) {
            log().error("Material does not secify a '{}' texture", it.texture_name);
            return nullptr;
        }

        Asset<Texture> texture;
        if(!texture.request(m_asset_mgr, texture_name)) {
            log().error("Can't find reqired '{}' texture for material", it.texture_name);
            return nullptr;
        }
        textures.push_back(std::move(texture));
    }
    mat->setTextures(std::move(textures));

    std::vector<byte> data;
    data.resize(m_material_uniform_buffer_size);
    for(auto& it : m_material_params) {
        readMaterialParameter(properties, it.first, it.second, &data[0]);
    }

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, mat->getUniformBufferObject());
    glBufferSubData(GL_UNIFORM_BUFFER, 0, m_material_uniform_buffer_size, &data[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return std::move(mat);
}

std::unique_ptr<DefaultGeometryRenderer> DefaultGeometryRenderer::createRenderer(
    xml::Element const* properties, AssetManager& asset_mgr, unsigned id
) {
    auto shader_params = properties->FirstChildElement("ShaderProgram");
    if(shader_params == nullptr) {
        log().error("Can't create DefaultGeometryRenderer: ShaderProgram parameter is missing");
        return nullptr;
    }

    Asset<ShaderProgram> shader;
    if(!shader.request(asset_mgr, shader_params)) {
        log().error("Could not load ShaderProgram for DefaultGeometryRenderer");
        return nullptr;
    }
    auto renderer = std::make_unique<DefaultGeometryRenderer>(asset_mgr, id);

    for(auto it : xml::IterateChildElements(properties, "RequiredTexture")) {
        std::string texture_name, sampler_uniform;
        xml::getAttribute(it, "texturen_name", texture_name);
        xml::getAttribute(it, "sampler_uniform_name", sampler_uniform);

        renderer->addRequiredTexture(texture_name, sampler_uniform);
    }

    if(!renderer->setShader(shader)) return nullptr;

    return std::move(renderer);
}

} // namespace bembel::graphics
