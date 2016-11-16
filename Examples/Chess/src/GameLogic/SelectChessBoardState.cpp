/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "SelectChessBoardState.h"
#include "../CameraControle.h"
#include "../Chess/ChessBoard.h"

#include <BembelGraphics/RenderingPipeline/RenderingPipeline.h>


/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

SelectChessBoardState::SelectChessBoardState(
	StateMashine* sm, 
	RenderingPipeline* rp, 
	CameraControle* cam)
	: GameState(sm)
	, _pipline(rp)
	, _cam(cam)
{}

SelectChessBoardState::~SelectChessBoardState()
{}

void SelectChessBoardState::AddChessBoard(ChessBoard* board, GameState* state)
{
	_boards.push_back(board);
	_states.push_back(state);
}

void SelectChessBoardState::OnNextButtonPress()
{
	_selection += 1;
	_selection %= _boards.size();

	OnBoardChange();
}

void SelectChessBoardState::OnPrevButtonPress()
{
	_selection += _boards.size()- 1;
	_selection %= _boards.size();

	OnBoardChange();
}

void SelectChessBoardState::OnSelectButtonPress()
{
	NextState(_states[_selection]);
}

void SelectChessBoardState::OnEnterState()
{
	OnBoardChange();
}

void SelectChessBoardState::OnExitState()
{
	_cam->EnableManualControle(true);
}

void SelectChessBoardState::Update(double time)
{
	_cam->SetYaw(_cam->GetYaw() + 0.5*time);
	_cam->SetPitch(0.1*sin(_cam->GetYaw()*1.83f)-0.3);	
}

void SelectChessBoardState::OnBoardChange()
{
	float w = _boards[_selection]->GetWidth();
	float h = _boards[_selection]->GetHeight();
	_pipline->SetScene(_boards[_selection]->GetScene());
	_cam->SetCameraOffset(glm::vec3(w, 0.5f, h));
	_cam->SetDist(1.5*sqrt(w*w +h*h));
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
