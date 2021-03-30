﻿#include "camera-controle.h"

#include <glfw/glfw3.h>

#include <glm/gtc/quaternion.hpp>
#include <iostream>

using namespace bembel;

CameraControle::CameraControle(
    kernel::EventManager& event_mgr, std::shared_ptr<graphics::Camera> camera)
: event_mgr(event_mgr)
, camera(camera) {
    this->event_mgr.addHandler<kernel::MouseButtonPressEvent>(this);
    this->event_mgr.addHandler<kernel::MouseButtonReleaseEvent>(this);
    this->event_mgr.addHandler<kernel::CursorMovedEvent>(this);
    this->event_mgr.addHandler<kernel::ScrollEvent>(this);

    this->move = false;
}

CameraControle::~CameraControle() {
    this->event_mgr.removeHandler<kernel::MouseButtonPressEvent>(this);
    this->event_mgr.removeHandler<kernel::MouseButtonReleaseEvent>(this);
    this->event_mgr.removeHandler<kernel::CursorMovedEvent>(this);
    this->event_mgr.removeHandler<kernel::ScrollEvent>(this);
}

void CameraControle::setPitch(float f) {
    this->pitch = f;
}

void CameraControle::setYaw(float f) {
    this->yaw = f;
}

void CameraControle::setDist(float f) {
    this->dist = f;
}

float CameraControle::getPitch() const {
    return this->pitch;
}

float CameraControle::getYaw() const {
    return this->yaw;
}

float CameraControle::getDist() const {
    return this->dist;
}

void CameraControle::enableManualControle(bool b) {
    this->enabeld = b;
}

void CameraControle::setCameraOffset(const glm::vec3& pos) {
    this->offset = pos;
}

void CameraControle::update(double dTime) {
    glm::vec2 cursor_movement = this->cursor_pos - this->prev_pos;
    this->prev_pos            = this->cursor_pos;

    if(this->enabeld) {
        if(this->move) {
            this->yaw -= 0.01f * (cursor_movement.x);
            this->pitch -= 0.01f * (cursor_movement.y);
            if(this->pitch <= -1.5f) this->pitch = -1.5f;
            if(this->pitch >= +1.5f) this->pitch = +1.5f;
        }
    }

    const static glm::vec3 X_AXIS = glm::vec3(1, 0, 0);
    const static glm::vec3 Y_AXIS = glm::vec3(0, 1, 0);
    const static glm::vec3 Z_AXIS = glm::vec3(0, 0, 1);

    glm::quat pitch = glm::angleAxis(this->pitch, X_AXIS);
    glm::quat yaw   = glm::angleAxis(this->yaw, Y_AXIS);
    this->camera->setOrientation(yaw * pitch);

    glm::vec3 pos = this->offset + this->dist * glm::mat3_cast(yaw * pitch) * Z_AXIS;
    this->camera->setPosition(pos);
}

void CameraControle::handleEvent(const kernel::MouseButtonPressEvent& event) {
    if(event.button_id == GLFW_MOUSE_BUTTON_RIGHT) this->move = true;
}

void CameraControle::handleEvent(const kernel::MouseButtonReleaseEvent& event) {
    if(event.button_id == GLFW_MOUSE_BUTTON_RIGHT) this->move = false;
}

void CameraControle::handleEvent(const kernel::CursorMovedEvent& event) {
    this->cursor_pos = event.position;
}

void CameraControle::handleEvent(const kernel::ScrollEvent& event) {
    this->dist += event.y;
    if(this->dist < 1) this->dist = 1.0f;
}
