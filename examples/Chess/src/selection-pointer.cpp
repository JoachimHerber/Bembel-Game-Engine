#include "selection-pointer.h"

#include "glfw/glfw3.h"

using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;

SelectionPointer::SelectionPointer(EventManager& event_mgr, GraphicSystem& grapic_sys, Scene* scene)
: event_mgr(event_mgr)
, grapic_sys(grapic_sys)
, scene(scene) {
    this->event_mgr.addHandler<CursorMovedEvent>(this);
    this->event_mgr.addHandler<MouseButtonPressEvent>(this);
}

SelectionPointer::~SelectionPointer() {
    this->event_mgr.removeHandler<CursorMovedEvent>(this);
    this->event_mgr.removeHandler<MouseButtonPressEvent>(this);
}

void SelectionPointer::handleEvent(const CursorMovedEvent& event) {
    glm::ivec2 windowSize = event.window->getWindowSize();
    glm::ivec2 fbSize     = event.window->getFrameBufferSize();

    glm::vec2 pos = event.position;

    // convert courser position form to window coordinates
    // to viewport coordinates.
    pos /= glm::vec2(windowSize);
    pos.y = 1 - pos.y;
    pos *= fbSize;

    this->updateRay(pos);
}

void SelectionPointer::handleEvent(const MouseButtonPressEvent& event) {
    if(event.button_id == GLFW_MOUSE_BUTTON_LEFT) this->select.emit();
}

const glm::vec3& SelectionPointer::getRayOrigin() const {
    return this->ray_origin;
}

const glm::vec3& SelectionPointer::getRayDirection() const {
    return this->ray_direction;
}

const glm::ivec2& SelectionPointer::getSelectedTile() const {
    return this->tile;
}

Signal<>& SelectionPointer::getSelectSignal() {
    return this->select;
}

void SelectionPointer::updateRay(glm::vec2 pos) {
    // search through rendering pipline
    for(const auto& pipline : this->grapic_sys.getRenderingPipelines()) {
        const glm::vec2 res = pipline->getResulution();
        if(pipline->getScene().get() != this->scene)
            continue; // rendering pipline is for an other scene

        for(const auto& it : pipline->getViews()) {
            auto vp = it->getViewport();
            if(!vp) continue;
            glm::vec2 vpPos = pos - glm::vec2(vp->getPosition());
            vpPos /= glm::vec2(vp->getSize());

            // test if the cursor is within the view port
            if(!(0.0f <= vpPos.x && vpPos.x <= 1.0f && 0.0f <= vpPos.y && vpPos.y <= 1.0f))
                continue;

            glm::vec2 tc =
                glm::vec2(it->getViewAreaPosition()) + glm::vec2(it->getViewAreaSize()) * vpPos;
            tc /= res;

            auto cam = pipline->getCamera();

            glm::vec4 rPos(2 * tc.x - 1, 2 * tc.y - 1, 0, 1);
            rPos = cam->getInverseProjectionMatrix() * rPos;
            rPos /= rPos.w;
            rPos = cam->getInverseViewMatrix() * rPos;

            this->ray_origin    = cam->getPosition();
            this->ray_direction = glm::vec3(rPos) - this->ray_origin;
            this->ray_direction = glm::normalize(this->ray_direction);
        }
    }
    float     f = -this->ray_origin.y / this->ray_direction.y;
    glm::vec3 p = this->ray_origin + f * this->ray_direction;
    p *= 0.5f;

    this->tile = glm::ivec2{p.x + 0.5f, p.z + 0.5f};
}
