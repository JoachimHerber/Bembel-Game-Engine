module;
#include <glbinding/gl/gl.h>
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace ::gl;

Material::Material(uint renderer, int size) : m_renderer(renderer) {
    glGenBuffers(1, &(m_uniform_buffer_object));
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uniform_buffer_object);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
}

Material::~Material() {
    glDeleteBuffers(1, &(m_uniform_buffer_object));
}

} // namespace bembel::graphics
