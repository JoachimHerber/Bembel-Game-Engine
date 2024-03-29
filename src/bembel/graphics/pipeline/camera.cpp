﻿module;
module bembel.graphics.pipeline;

import bembel.base;

namespace bembel::graphics {
using namespace bembel::base;

void Camera::setUpProjection(float fildOfFiew, float aspectRation, float near, float far) {
    m_proj_matrix         = glm::perspective(fildOfFiew, aspectRation, near, far);
    m_inverse_proj_matrix = glm::inverse(m_proj_matrix);

    float const y         = glm::sin(fildOfFiew);
    float const x         = y * aspectRation;
    float const z         = glm::cos(fildOfFiew);
    m_view_frustum.left   = glm::normalize(vec4(-x, 0.f, z, 0.f)); //   left clipping plane
    m_view_frustum.right  = glm::normalize(vec4(+x, 0.f, z, 0.f)); //  right clipping plane
    m_view_frustum.bottom = glm::normalize(vec4(0.f, -y, z, 0.f)); // bottom clipping plane
    m_view_frustum.top    = glm::normalize(vec4(0.f, +y, z, 0.f)); //    top clipping plane
    m_view_frustum.near   = vec4(0.f, 0.f, +1.f, -near);           //   near clipping plane
    m_view_frustum.far    = vec4(0.f, 0.f, -1.f, +far);            //    far clipping plane
}

void Camera::setOrtho(float left, float right, float bottom, float top, float near, float far) {
    m_proj_matrix         = glm::ortho(left, right, bottom, top, near, far);
    m_inverse_proj_matrix = glm::inverse(m_proj_matrix);

    m_view_frustum.left   = vec4(-1.f, -0.f, -0.f, -left);   //   left clipping plane
    m_view_frustum.right  = vec4(+1.f, +0.f, +0.f, +right);  //  right clipping plane
    m_view_frustum.bottom = vec4(-0.f, -1.f, -0.f, -bottom); // bottom clipping plane
    m_view_frustum.top    = vec4(+0.f, +1.f, +0.f, +top);    //    top clipping plane
    m_view_frustum.near   = vec4(-0.f, -0.f, -1.f, -near);   //   near clipping plane
    m_view_frustum.far    = vec4(+0.f, +0.f, +1.f, +far);    //    far clipping plane
}

void Camera::updateViewMatrix() {
    m_view_matrix         = glm::translate(mat4_cast(inverse(m_oriantation)), -m_position);
    m_inverse_view_matrix = glm::affineInverse(m_view_matrix);
}

} // namespace bembel::graphics
