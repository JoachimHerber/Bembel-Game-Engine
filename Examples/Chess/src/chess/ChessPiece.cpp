/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "ChessPiece.h"
#include "ChessPieceType.h"
#include "ChessBoard.h"
#include "Player.h"

#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Scene/GeometryComponent.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

ChessPiece::ChessPiece(
	ChessPieceType* type,
	Player* owner,
	const glm::ivec2& startPos)
	: _scene(owner->GetChessBoard()->GetScene().get())
	, _type(type)
	, _originalType(type)
	, _owner(owner)
	, _position(startPos)
	, _startPositon(startPos)
	, _isAlive(false)
	, _entity(Scene::INVALID_ENTITY)
{
	_entity = _scene->CreateEntity();

	auto posComp = _scene->CreateComponent<PositionComponent>(_entity);
	posComp->position = 2.0f*glm::vec3(_position.x, 0, _position.y);

	auto geomComp = _scene->CreateComponent<GeometryComponent>(_entity);
	geomComp->model = _type->GetModle(_owner);
}

void ChessPiece::Promote(ChessPieceType* type)
{
	_type = type;
	auto geomComp = _scene->GetComponent<GeometryComponent>(_entity);
	geomComp->model = _type->GetModle(_owner);
}

ChessPieceType* ChessPiece::GetType() const
{
	return _type;
}

Player* ChessPiece::GetOwner() const
{
	return _owner;
}

ChessBoard* ChessPiece::GetBoard() const
{
	return _owner->GetChessBoard();
}

const glm::ivec2& ChessPiece::GetPositon() const
{
	return _position;
}

void ChessPiece::SetPosition(const glm::ivec2& pos)
{
	_hasMoved = true;
	_position = pos;

	auto posComp = _scene->GetComponent<PositionComponent>(_entity);
	posComp->position = 2.0f*glm::vec3(_position.x, 0, _position.y);
}

Scene::EntityID ChessPiece::GetEntity()
{
	return _entity;
}

bool ChessPiece::IsAlive() const
{
	return _isAlive;
}

void ChessPiece::Kill(Player* killer)
{
	_isAlive = true; 
	_position.x = -1;
	_position.y = -1;
	_owner->RemoveChessPiece(this);
	killer->CaptureChessPiece(this);
}

void ChessPiece::Reset()
{
	SetPosition(_startPositon);
	_isAlive = true;
	_hasMoved = false;

	_type = _originalType;

	auto geomComp = _scene->GetComponent<GeometryComponent>(_entity);
	geomComp->model = _type->GetModle(_owner);
}

bool ChessPiece::HasMoved() const
{
	return _hasMoved;
}

void ChessPiece::UpdatePossibleMoves()
{
	_possibleMoves.clear();
	if (_isAlive)
	{
		_type->GetMoveSet().GetAvailableMoves(
			this, _possibleMoves);
	}
}

const std::vector<MoveSet::Move>& ChessPiece::GetPossibleMoves() const
{
	return _possibleMoves;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
