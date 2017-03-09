/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "selection-pointer.h"

#include "glfw/glfw3.h"

#include <bembel-kernel/display/window.h>
#include <bembel-graphics/graphic-system.h>
#include <bembel-graphics/rendering-pipeline/camera.h>
#include <bembel-graphics/rendering-pipeline/rendering-pipeline.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
SelectionPointer::SelectionPointer(
	bembel::EventManager* eventMge,
	bembel::GraphicSystem* grapicSys,
	bembel::Scene* scene )
	: _eventMgr( eventMge )
	, _grapicSys( grapicSys )
	, _scene( scene )
{
	_eventMgr->AddHandler<bembel::CursorMovedEvent>( this );
	_eventMgr->AddHandler<bembel::MouseButtonPressEvent>( this );
}

SelectionPointer::~SelectionPointer()
{
	_eventMgr->RemoveHandler<bembel::CursorMovedEvent>( this );
	_eventMgr->RemoveHandler<bembel::MouseButtonPressEvent>( this );
}

void SelectionPointer::HandleEvent( const bembel::CursorMovedEvent& event )
{
	glm::ivec2 windowSize = event.window->GetWindowSize();
	glm::ivec2 fbSize = event.window->GetFrameBufferSize();

	glm::vec2 pos = event.position;

	// convert courser position form to window coordinates 
	// to viewport coordinates.
	pos /= glm::vec2( windowSize );
	pos.y = 1-pos.y;
	pos *= fbSize;

	for( const auto& viewPort : _grapicSys->GetViewports(
		event.window->GetWindowID() ) )
	{
		glm::vec2 vpPos = pos - glm::vec2( viewPort->GetPosition() );
		vpPos /= glm::vec2( viewPort->GetSize() );

		// test if the cursor is within the view port
		if( 0.0f <= vpPos.x && vpPos.x <= 1.0f &&
			0.0f <= vpPos.y && vpPos.y <= 1.0f )
		{
			UpdateRay( viewPort->GetView().get(), vpPos );
		}
	}

}

void SelectionPointer::HandleEvent( const bembel::MouseButtonPressEvent& event )
{
	if( event.buttonID == GLFW_MOUSE_BUTTON_LEFT )
		_select.Emit();
}

const glm::vec3 & SelectionPointer::GetRayOrigin() const
{
	return _rayOrigin;
}

const glm::vec3 & SelectionPointer::GetRayDirection() const
{
	return _rayDirection;
}

const glm::ivec2 & SelectionPointer::GetSelectedTile() const
{
	return _tile;
}

bembel::Signal<>& SelectionPointer::GetSelectSignal()
{
	return _select;
}

void SelectionPointer::UpdateRay(
	bembel::Viewport::View* view, const glm::vec2& pos )
{
	// search through rendering pipline
	for( const auto& pipline: _grapicSys->GetRenderingPiplies() )
	{
		const glm::vec2 res = pipline->GetResulution();
		if( pipline->GetScene().get() != _scene )
			continue; // rendering pipline is for an other scene

		for( const auto& it : pipline->GetViews() )
		{
			if( view != it.get() )
				continue; // not the view we are looking for

			glm::vec2 tc = it->GetViewAreaMin() +
				pos * (it->GetViewAreaMax() - it->GetViewAreaMin());

			auto cam = pipline->GetCamera();

			glm::vec4 rPos( 2*tc.x - 1, 2*tc.y - 1, 0, 1 );
			rPos = cam->GetinverseProjectionMatrix()*rPos;
			rPos /= rPos.w;
			rPos = cam->GetinverseViewMatrix()*rPos;

			_rayOrigin     = cam->GetPosition();
			_rayDirection = glm::vec3( rPos ) - _rayOrigin;
			_rayDirection = glm::normalize( _rayDirection );
		}
	}
	float f = -_rayOrigin.y/_rayDirection.y;
	glm::vec3 p = _rayOrigin + f*_rayDirection;
	p *= 0.5f;

	_tile = glm::ivec2{p.x + 0.5f, p.z + 0.5f};
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
