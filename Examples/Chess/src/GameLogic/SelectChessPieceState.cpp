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
	_selectedChessPiece += 1;
	_selectedChessPiece %= _player->GetChessPieces().size();

	OnChessPieceChanged();
}

void SelectChessPieceState::OnPrevButtonPress()
{
	_selectedChessPiece += _player->GetChessPieces().size() -1;
	_selectedChessPiece %= _player->GetChessPieces().size();
	
	OnChessPieceChanged();
}

void SelectChessPieceState::OnSelectButtonPress()
{

}

void SelectChessPieceState::OnEnterState()
{
	_selectedChessPiece = 0;
	_player->GetChessBoard()->UpdatePossibleMoves();
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
	auto piece = _player->GetChessPieces()[_selectedChessPiece];
	auto  posContainer = scene->RequestComponentContainer<PositionComponent>();
	auto& positions = posContainer->GetComponents();

	positions[piece->GetEntity()].position.y =
		0.3f + 0.3f*std::sin(_time);

	auto board = _player->GetChessBoard();
	for (const auto& move : piece->GetPossibleMoves())
	{
		auto tile = board->GetTileEntity(move.target);

		if(tile != Scene::INVALID_ENTITY)
			positions[tile].position.y =
			  0.1f + 0.05f*std::sin(
				  0.7836*_time +
				  1.678*move.target.x + 
				  3.764*move.target.y);
	}
}


void SelectChessPieceState::OnChessPieceChanged()
{
	auto  scene        = _player->GetChessBoard()->GetScene();
	auto  posContainer = scene->RequestComponentContainer<PositionComponent>();
	auto& positions    = posContainer->GetComponents();

	for (auto& it : _player->GetChessPieces())
		positions[it->GetEntity()].position.y = 0;

	auto board = _player->GetChessBoard();
	for (int u = 0; u < board->GetWidth(); ++u)
	{
		for (int v = 0; v < board->GetHeight(); ++v)
		{
			if (!board->IsPositionValid({u,v}))
				continue;

			positions[board->GetTileEntity({u,v})].position.y = 0;
		}
	}
	_time = 0.0f;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
