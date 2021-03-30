#ifndef BEMBEL_SELECTIONCURSOR_H
#define BEMBEL_SELECTIONCURSOR_H

#include <bembel/bembel.hpp>
#include <glm/glm.hpp>

class SelectionPointer {
  public:
    SelectionPointer(bembel::EventManager&, bembel::GraphicSystem&, bembel::Scene*);
    ~SelectionPointer();

    void handleEvent(const bembel::CursorMovedEvent& event);
    void handleEvent(const bembel::MouseButtonPressEvent& event);

    const glm::vec3& getRayOrigin() const;
    const glm::vec3& getRayDirection() const;

    const glm::ivec2& getSelectedTile() const;

    bembel::base::Signal<>& getSelectSignal();

  private:
    void updateRay(glm::vec2 pos);

  private:
    bembel::EventManager&  event_mgr;
    bembel::GraphicSystem& grapic_sys;
    bembel::Scene*         scene;

    glm::vec3 ray_origin;
    glm::vec3 ray_direction;

    glm::ivec2 tile;

    bembel::Signal<> select;
};

#endif // include guards
