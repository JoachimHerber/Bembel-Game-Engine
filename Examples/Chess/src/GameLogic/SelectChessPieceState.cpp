/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "SelectChessPieceState.h"
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

SelectChessPieceState::SelectChessPieceState(StateMashine* sm, Player* player )
	: GameState(sm)
	, _player(player)
{}

SelectChessPieceState::~SelectChessPieceState()
{}

void SelectChessPieceState::OnNextButtonPress()
{
	auto scene = _player->GetChessBoard()->GetScene();
	auto piece = _player->GetChessPieces()[_selectedChessPiece]->GetEntity();
	auto pos = scene->GetComponent<PositionComponent>(piece);

	pos->position.y = 0.0f;
	_time = 0.0f;

	_selectedChessPiece += 1;
	_selectedChessPiece %= _player->GetChessPieces().size();
}

void SelectChessPieceState::OnPrevButtonPress()
{
	auto scene = _player->GetChessBoard()->GetScene();
	auto piece = _player->GetChessPieces()[_selectedChessPiece]->GetEntity();
	auto pos = scene->GetComponent<PositionComponent>(piece);

	pos->position.y = 0.0f;
	_time = 0.0f;

	_selectedChessPiece += _player->GetChessPieces().size() -1;
	_selectedChessPiece %= _player->GetChessPieces().size();
}

void SelectChessPieceState::OnSelectButtonPress()
{

}

void SelectChessPieceState::OnEnterState()
{
	_selectedChessPiece = 0;
}

void SelectChessPieceState::OnExitState()
{
	auto scene = _player->GetChessBoard()->GetScene();
	auto piece = _player->GetChessPieces()[_selectedChessPiece]->GetEntity();
	auto pos = scene->GetComponent<PositionComponent>(piece);

	pos->position.y = 0.0f;
}

void SelectChessPieceState::Update(double time)
{
	_time += time*2*3.14159265359;

	auto scene = _player->GetChessBoard()->GetScene();
	auto piece = _player->GetChessPieces()[_selectedChessPiece]->GetEntity();
	auto pos   = scene->GetComponent<PositionComponent>(piece);

	pos->position.y = 0.3f + 0.3f*std::sin(_time);
}


} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
