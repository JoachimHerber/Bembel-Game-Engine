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

#include "select-move.h"
#include "perform-move.h"
#include "selection-pointer.h"

#include "../chess-game.h"
#include "../chess-piece.h"
#include "../../selection-component.h"

#include <bembel-kernel/scene/scene.h>

#include <iostream>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
SelectMoveState::SelectMoveState( ChessGame* game, SelectionPointer* pointer )
	: GameState( game )
	, _pointer( pointer )
{}

SelectMoveState::~SelectMoveState()
{}

void SelectMoveState::SetPerformMoveState( PerformMoveState* state )
{
	_performMove = state;
}

void SelectMoveState::SetChessPiece( ChessPiece* piece )
{
	_chessPiece = piece;
}

void SelectMoveState::OnEnterState()
{
	if( _chessPiece )
	{
		auto entity = _chessPiece->GetEntity();
		auto selectComp = _game->GetScene()->GetComponent<SelectionComponent>( entity );
		selectComp->state = SelectionComponent::SELECTED;
	}
	_pointer->GetSelectSignal().AddSlot( this, &SelectMoveState::SelectedMove );
}

void SelectMoveState::OnExitState()
{
	if( _chessPiece )
	{
		auto entity = _chessPiece->GetEntity();
		auto selectComp = _game->GetScene()->GetComponent<SelectionComponent>( entity );
		selectComp->state = SelectionComponent::UNSELECTABLE;

		for( auto& it : _chessPiece->GetPossibleMoves() )
		{
			glm::ivec2 target = it.move->GetTargetPosition(
				_chessPiece, it.param );

			selectComp = _game->GetBoardTileSelectionComponent( target );
			selectComp->state = SelectionComponent::UNSELECTABLE;
		}
	}
	_pointer->GetSelectSignal().RemoveSlot( this, &SelectMoveState::SelectedMove );
}

void SelectMoveState::Update( double time )
{
	glm::ivec2 tile = _pointer->GetSelectedTile();

	if( tile.x >= 0 && tile.x < 8 && tile.y >= 0 && tile.y < 8 )
	{
		for( size_t n = 0; n < _chessPiece->GetPossibleMoves().size(); ++n )
		{
			auto& it = _chessPiece->GetPossibleMoves()[n];
			glm::ivec2 target = it.move->GetTargetPosition( _chessPiece, it.param );

			if( target == tile )
			{
				UpdateSelection( n );
				return;
			}
		}
	}
	UpdateSelection( -1 );
}

void SelectMoveState::SelectedMove()
{
	if( _selctedMove >= 0 )
	{
		_performMove->SetMove( 
			_chessPiece,
			_chessPiece->GetPossibleMoves()[_selctedMove] );
		NextState( _performMove );
		_selctedMove = -1;
	}
}

void SelectMoveState::UpdateSelection(int i)
{
	if( _selctedMove >= 0 )
	{
		auto& move = _chessPiece->GetPossibleMoves()[_selctedMove];
		glm::ivec2 target = move.move->GetTargetPosition( _chessPiece, move.param );

		auto selectComp = _game->GetBoardTileSelectionComponent( target );
		selectComp->state = SelectionComponent::SELECTABLE;
	}
	_selctedMove = i;
	if( _selctedMove >= 0 )
	{
		auto& move = _chessPiece->GetPossibleMoves()[_selctedMove];
		glm::ivec2 target = move.move->GetTargetPosition( _chessPiece, move.param );

		auto selectComp = _game->GetBoardTileSelectionComponent( target );
		selectComp->state = SelectionComponent::FOCUSED;
	}
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
