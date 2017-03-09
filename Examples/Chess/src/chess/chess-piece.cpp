/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "chess-piece.h"
#include "chess-piece-type.h"
#include "player.h"

#include "../selection-component.h"

#include <bembel-kernel/scene/scene.h>
#include <bembel-kernel/scene/position-component.h>
#include <bembel-graphics/geometry/geometry-component.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
ChessPiece::ChessPiece(
	ChessPieceType* type,
	bembel::Scene* scene,
	unsigned owner,
	const glm::ivec2& startPos)
	: _scene(scene)
	, _type(type)
	, _originalType(type)
	, _owner(owner)
	, _position(startPos)
	, _startPositon(startPos)
	, _isAlive(false)
	, _entity( bembel::Scene::INVALID_ENTITY)
{
	_entity = _scene->CreateEntity();

	auto posComp = _scene->CreateComponent<bembel::PositionComponent>(_entity);
	posComp->position = 2.0f*glm::vec3(_position.x, 0, _position.y);

	auto geomComp = _scene->CreateComponent<bembel::GeometryComponent>(_entity);
	geomComp->model = _type->GetModles()[_owner];

	auto selectComp = _scene->CreateComponent<SelectionComponent>(_entity);
	selectComp->state = SelectionComponent::UNSELECTABLE;
	//selectComp->stat = SelectionComponent::SELECTABLE;
}

void ChessPiece::Promote(ChessPieceType* type)
{
	_type = type;
	auto geomComp = _scene->GetComponent<bembel::GeometryComponent>(_entity);
	geomComp->model = _type->GetModles()[_owner];
}

ChessPieceType* ChessPiece::GetType() const
{
	return _type;
}

bembel::Scene * ChessPiece::GetScene() const
{
	return _scene;
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

	auto posComp = _scene->GetComponent<bembel::PositionComponent>(_entity);
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
	auto posComp = _scene->GetComponent<bembel::PositionComponent>( _entity );
	posComp->position = 2.0f*glm::vec3( _position.x, -1000, _position.y );
	_isAlive = true; 
}

void ChessPiece::Reset()
{
	SetPosition(_startPositon);
	_isAlive = true;
	_hasMoved = false;

	_type = _originalType;

	auto geomComp = _scene->GetComponent<bembel::GeometryComponent>(_entity);
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
