#ifndef BEMBEL_SELECTIONCURSOR_H
#define BEMBEL_SELECTIONCURSOR_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <bembel-open-gl.h>

#include <bembel-graphics/viewport.h>
#include <bembel-interaction/signal.hpp>
#include <bembel-kernel/events/input-events.h>

#include <glm/glm.hpp>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class EventManager;
class GraphicSystem;
class Scene;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class SelectionPointer
{
public:
	SelectionPointer( bembel::EventManager*, bembel::GraphicSystem*, bembel::Scene* scene );
	~SelectionPointer();

	void HandleEvent( const bembel::CursorMovedEvent& event );
	void HandleEvent( const bembel::MouseButtonPressEvent& event );

	const glm::vec3& GetRayOrigin()   const;
	const glm::vec3& GetRayDirection() const;

	const glm::ivec2& GetSelectedTile() const;

	bembel::Signal<>& GetSelectSignal();

private:
	void UpdateRay( bembel::Viewport::View* view, const glm::vec2& pos );

private:
	bembel::EventManager*   _eventMgr;
	bembel::GraphicSystem*  _grapicSys;
	bembel::Scene*          scene_;

	glm::vec3 _rayOrigin;
	glm::vec3 _rayDirection;

	glm::ivec2 _tile;

	bembel::Signal<> _select;
};

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
