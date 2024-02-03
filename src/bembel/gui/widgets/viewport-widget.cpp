module;
#include <cassert>
#include <filesystem>
#include <numbers>
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

ViewportWidget::ViewportWidget(In<Widget*> parent, Viewport* viewport, Camera* camera)
  : Widget{parent}, m_viewport{viewport}, m_camera{camera} {
    m_interaction_handles.push_back(&m_handle);

    this->size.change_signal.bind(this, &ViewportWidget::onSizeOrPositionChanged);
    this->position.change_signal.bind(this, &ViewportWidget::onSizeOrPositionChanged);
    m_handle.dragging_signal.bind(this, &ViewportWidget::onHandleMoved);

    m_handle.cursor = Asset<CursorIcon>("Hand");
}

ViewportWidget::~ViewportWidget() {}

void ViewportWidget::setCamera(Camera* camera) {
    m_camera = camera;
    if(m_camera) {
        m_camera->setUpProjection(
            60.0f * std::numbers::pi_v<float> / 180.0f,
            float(size.get().x) / float(size.get().y),
            0.1f,
            1000.0f
        );

        quat pitch = glm::angleAxis(m_camera_pitch, vec3(1, 0, 0));
        quat yaw   = glm::angleAxis(m_camera_yaw, vec3(0, 1, 0));
        m_camera->setOrientation(yaw * pitch);

        vec3 pos = m_camera_dist * mat3_cast(yaw * pitch) * vec3(0, 0, 1);
        m_camera->setPosition(pos);
    }
}
bool ViewportWidget::configure(xml::Element const* properties) {
    Widget::configure(properties);
    uint windowId, viewportId;
    if(xml::getAttribute(properties, "window", windowId)
       && xml::getAttribute(properties, "viewport", viewportId)) {
        auto window = getGUI()->engine->display.getWindow(windowId);

        if(window && window->getViewports().size() > viewportId) {
            m_viewport = window->getViewports()[viewportId].get();
        }
    }
    return true;
}

uint ViewportWidget::getMinWidth(In<std::optional<uint>>) const {
    return 0;
}

uint ViewportWidget::getMinHeight(In<std::optional<uint>>) const {
    return 0;
}

void ViewportWidget::onSizeOrPositionChanged(In<ivec2>, In<ivec2>) {
    auto size = this->size.get();
    auto pos  = this->position.get();

    m_handle.size = size;

    if(!m_viewport) return;

    for(auto parent = m_parent; parent; parent = parent->getParentWidget()) {
        pos += parent->position.get();
    }
    m_viewport->setPosition(pos);
    m_viewport->setSize(size);

    if(m_camera) {
        m_camera->setUpProjection(
            60.0f * std::numbers::pi_v<float> / 180.0f, float(size.x) / float(size.y), 0.1f, 1000.0f
        );
    }
}

void ViewportWidget::onHandleMoved(In<ivec2> cursor, InOut<ivec2> movement) {
    if(!m_camera) return;

    m_camera_yaw -= 0.004f * movement.x;
    m_camera_pitch += 0.002f * movement.y;
    if(m_camera_pitch <= -1.5f) m_camera_pitch = -1.5f;
    if(m_camera_pitch >= +1.5f) m_camera_pitch = +1.5f;
    movement = ivec2(0, 0);

    quat pitch = glm::angleAxis(m_camera_pitch, vec3(1, 0, 0));
    quat yaw   = glm::angleAxis(m_camera_yaw, vec3(0, 1, 0));
    m_camera->setOrientation(yaw * pitch);

    vec3 pos = m_camera_dist * mat3_cast(yaw * pitch) * vec3(0, 0, 1);
    m_camera->setPosition(pos);
}

} // namespace bembel::gui
