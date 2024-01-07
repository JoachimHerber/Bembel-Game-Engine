module;
#include <glbinding/gl/gl.h>

#include <memory>
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace ::gl;

Material::Material() {
    glGenBuffers(1, &(m_uniform_buffer_object));
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uniform_buffer_object);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Params), &m_params, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Material::~Material() {
    glDeleteBuffers(1, &(m_uniform_buffer_object));
}

void Material::setEmission(In<vec3> emission) {
    m_params.emission = vec4(emission, 1.0f);
    m_params_dirty    = true;
}

void Material::setBaseColor(In<vec3> base_color) {
    m_params.base_color = vec4(base_color, 1.0f);
    m_params_dirty      = true;
}

void Material::setRoughness(In<float> roughness) {
    m_params.roughness = roughness;
    m_params_dirty     = true;
}

void Material::setMetallic(In<float> metallic) {
    m_params.metallic = metallic;
    m_params_dirty    = true;
}

void Material::setF0(In<float> f0) {
    m_params.f0    = f0;
    m_params_dirty = true;
}

void Material::bindUniformBufferObject() {
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uniform_buffer_object);
    if(m_params_dirty) {
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Params), &m_params);
        m_params_dirty = false;
    }
}

std::unique_ptr<Material> Material::createAsset(xml::Element const* properties) {
    auto mat = std::make_unique<Material>();
    return mat;
}

bool Material::verifyUniformBlockLayout(ShaderProgram* shader) {
    if(shader == nullptr) return false;

    auto uniform_block_index = shader->getUniformBlockIndex("Material");
    auto uniform_buffer_size = shader->getUniformBlockDataSize(uniform_block_index);

    if(uniform_buffer_size != sizeof(Params)) {
        logError(
            "UniformBlockDataSize ({}) dosen't match expected size ({}) ",
            uniform_buffer_size,
            sizeof(Params)
        );
        return false;
    }

    std::vector<GLint> active_uniform_indices;
    shader->getUniformBlockActiveUniformIndices(uniform_block_index, &active_uniform_indices);

    for(GLint uniform_index : active_uniform_indices) {
        auto          offset = shader->getActiveUniformOffset(uniform_index);
        std::string   name;
        int           size;
        uint          type;
        shader->getActiveUniform(uniform_index, &size, &type, &name);

        switch(offset) {
            case 0:
            case 16:
                if(type != uint(GL_FLOAT_VEC4) || size != 1) return false;
                break;
            case 32:
            case 36:
            case 40:
                if(type != uint(GL_FLOAT) || size != 1) return false;
                break;
            default: return false;
        }
    }
    return true;
}

} // namespace bembel::graphics
