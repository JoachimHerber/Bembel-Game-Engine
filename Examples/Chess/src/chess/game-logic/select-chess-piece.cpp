/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "select-chess-piece.h"
#include "selection-pointer.h"
#include "select-move.h"

#include "../chess-game.h"
#include "../chess-piece.h"
#include "../../selection-component.h"

#include <bembel-kernel/scene/scene.h>

#include <iostream>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
SelectChessPieceState::SelectChessPieceState( ChessGame* game, unsigned player, SelectionPointer* pointer )
	: GameState( game )
	, _player( player )
	, _pointer( pointer )
{}

SelectChessPieceState::~SelectChessPieceState()
{}

void SelectChessPieceState::SetSelectMoveState( SelectMoveState* state )
{
	_selectMove = state;
}

void SelectChessPieceState::OnEnterState()
{
	_game->UpdatePossibleMoves();
	_pointer->GetSelectSignal().AddSlot( this, &SelectChessPieceState::SelectChessPiece);
}

void SelectChessPieceState::OnExitState()
{
	_pointer->GetSelectSignal().RemoveSlot( this, &SelectChessPieceState::SelectChessPiece );
}

void SelectChessPieceState::Update( double time )
{

	glm::ivec2 tile = _pointer->GetSelectedTile();

	if( tile.x >= 0 && tile.x < 8 && tile.y >= 0 && tile.y < 8 )
	{
		auto* chessPice = _game->GetChessBoard()[tile.x][tile.y];
		if( chessPice && chessPice->GetOwner() == _player )
		{
			if( !chessPice->GetPossibleMoves().empty() )
			{
				SetSelectedChessPice(chessPice);
				return;
			}
		}
	}

	SetSelectedChessPice( nullptr );
}

void SelectChessPieceState::SelectChessPiece()
{
	if( _selectedChessPiece )
	{
		_selectMove->SetChessPiece(_selectedChessPiece);
		NextState(_selectMove);
	}
}

void SelectChessPieceState::SetSelectedChessPice( ChessPiece* pice )
{
	if( _selectedChessPiece )
	{
		auto entity = _selectedChessPiece->GetEntity();
		auto selectComp = _game->GetScene()->GetComponent<SelectionComponent>( entity );
		selectComp->state = SelectionComponent::UNSELECTABLE;

		for( auto& it : _selectedChessPiece->GetPossibleMoves() )
		{
			glm::ivec2 target = it.move->GetTargetPosition(
				_selectedChessPiece, it.param);

			selectComp = _game->GetBoardTileSelectionComponent( target );
			selectComp->state = SelectionComponent::UNSELECTABLE;
		}
	}
	_selectedChessPiece = pice;
	if( _selectedChessPiece )
	{
		auto entity = _selectedChessPiece->GetEntity();
		auto selectComp = _game->GetScene()->GetComponent<SelectionComponent>( entity );
		selectComp->state = SelectionComponent::FOCUSED;

		for( auto& it : _selectedChessPiece->GetPossibleMoves() )
		{
			glm::ivec2 target = it.move->GetTargetPosition(
				_selectedChessPiece, it.param );

			selectComp = _game->GetBoardTileSelectionComponent( target );
			selectComp->state = SelectionComponent::SELECTABLE;
		}
	}
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
