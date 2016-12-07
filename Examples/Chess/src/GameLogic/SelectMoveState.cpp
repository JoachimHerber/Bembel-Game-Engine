/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "SelectMoveState.h"
#include "PerformMoveState.h"
#include "../chess/Player.h"
#include "../chess/ChessPiece.h"
#include "../chess/ChessBoard.h"

#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/PositionComponent.h>

#include <Bembelnteraction/Input/Button.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

SelectMoveState::SelectMoveState(
	StateMashine* sm, 
	ChessBoard* board)
	: GameState(sm)
	, _board(board)
{}

SelectMoveState::~SelectMoveState()
{}

void SelectMoveState::Init(PerformMoveState* state)
{
	_performMove = state;
}

void SelectMoveState::OnNextButtonPress()
{
	_selectedMove += 1;
	_selectedMove %= _chessPiece->GetPossibleMoves().size();

	OnMoveChanged();
}

void SelectMoveState::OnPrevButtonPress()
{
	_selectedMove += _chessPiece->GetPossibleMoves().size() -1;
	_selectedMove %= _chessPiece->GetPossibleMoves().size();
	
	OnMoveChanged();
}

void SelectMoveState::OnSelectButtonPress()
{
	NextState(_performMove);
}

void SelectMoveState::OnEnterState()
{
	_selectedMove = 0;
	OnMoveChanged();
}

void SelectMoveState::OnExitState()
{
	_performMove->SetMove(
		_chessPiece,
		_chessPiece->GetPossibleMoves()[_selectedMove]
	);
}

void SelectMoveState::Update(double time)
{
	_time += time*2*3.14159265359;

	auto scene = _board->GetScene();
	auto  posContainer = scene->RequestComponentContainer<PositionComponent>();
	auto& positions = posContainer->GetComponents();

	positions[_chessPiece->GetEntity()].position.y =
		0.1f + 0.05f*std::sin(_time);

	for (size_t n = 0; n < _chessPiece->GetPossibleMoves().size(); ++n)
	{
		auto& move = _chessPiece->GetPossibleMoves()[n];
		glm::ivec2 target = move.move->GetTargetPosition(_chessPiece, move.param);
		auto tile = _board->GetTileEntity(target);

		if (tile != Scene::INVALID_ENTITY)
		{
			if (n == _selectedMove)
			{
				positions[tile].position.y =
					0.2f + 0.1f*std::sin(
						1.345*_time +
						1.678*target.x +
						3.764*target.y);
			}
			else
			{
				positions[tile].position.y =
					0.02f + 0.01f*std::sin(
						0.7836*_time +
						1.678*target.x +
						3.764*target.y);
			}
		}
	}
}

void SelectMoveState::SetChessPiece(ChessPiece* pice)
{
	_chessPiece = pice;
}

void SelectMoveState::OnMoveChanged()
{

}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
