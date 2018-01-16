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

#include "chess-piece.h"
#include "chess-piece-type.h"
#include "player.h"

#include "../selection-component.h"

#include <bembel-kernel/scene/scene.h>
#include <bembel-kernel/scene/position-component.h>
#include <bembel-kernel/scene/rotation-component.h>
#include <bembel-graphics/geometry/geometry-component.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
ChessPiece::ChessPiece(
	ChessPieceType* type,
	bembel::Scene* scene,
	unsigned owner,
	const glm::ivec2& startPos)
	: scene_(scene)
	, _type(type)
	, _originalType(type)
	, _owner(owner)
	, _position(startPos)
	, _startPositon(startPos)
	, _isAlive(false)
	, _entity( bembel::Scene::INVALID_ENTITY)
{
	_entity = scene_->CreateEntity();

	auto posComp = scene_->CreateComponent<bembel::PositionComponent>(_entity);
	posComp->position = 2.0f*glm::vec3(_position.x, 0, _position.y);

	auto rotComp = scene_->CreateComponent<bembel::RotationComponent>(_entity);
	rotComp->rotation = glm::angleAxis(glm::radians(_owner == 0 ? 180.f : 1.0f), glm::vec3(0,1,0));

	auto geomComp = scene_->CreateComponent<bembel::GeometryComponent>(_entity);
	geomComp->model = _type->GetModles()[_owner];

	auto selectComp = scene_->CreateComponent<SelectionComponent>(_entity);
	selectComp->state = SelectionComponent::UNSELECTABLE;
	//selectComp->stat = SelectionComponent::SELECTABLE;
}

void ChessPiece::Promote(ChessPieceType* type)
{
	_type = type;
	auto geomComp = scene_->GetComponent<bembel::GeometryComponent>(_entity);
	geomComp->model = _type->GetModles()[_owner];
}

ChessPieceType* ChessPiece::GetType() const
{
	return _type;
}

bembel::Scene * ChessPiece::GetScene() const
{
	return scene_;
}

unsigned ChessPiece::GetOwner() const
{
	return _owner;
}

const glm::ivec2& ChessPiece::GetPositon() const
{
	return _position;
}

void ChessPiece::SetPosition(const glm::ivec2& pos)
{
	_hasMoved = true;
	_position = pos;

	auto posComp = scene_->GetComponent<bembel::PositionComponent>(_entity);
	posComp->position = 2.0f*glm::vec3(_position.x, 0, _position.y);
}

bembel::Scene::EntityID ChessPiece::GetEntity()
{
	return _entity;
}

bool ChessPiece::IsAlive() const
{
	return _isAlive;
}

void ChessPiece::Kill()
{
	auto posComp = scene_->GetComponent<bembel::PositionComponent>( _entity );
	posComp->position = 2.0f*glm::vec3( _position.x, -1000, _position.y );
	_isAlive = true; 
}

void ChessPiece::Reset()
{
	SetPosition(_startPositon);
	_isAlive = true;
	_hasMoved = false;

	_type = _originalType;

	auto geomComp = scene_->GetComponent<bembel::GeometryComponent>(_entity);
	geomComp->model = _type->GetModles()[_owner];
}

bool ChessPiece::HasMoved() const
{
	return _hasMoved;
}

void ChessPiece::UpdatePossibleMoves(const ChessBoard& board)
{
	_possibleMoves.clear();
	if (_isAlive)
	{
		_type->GetMoveSet().GetAvailableMoves(
			this, board, _possibleMoves);
	}
}

const std::vector<MoveSet::Move>& ChessPiece::GetPossibleMoves() const
{
	return _possibleMoves;
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
