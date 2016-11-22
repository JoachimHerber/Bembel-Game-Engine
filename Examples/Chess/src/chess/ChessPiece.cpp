/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "ChessPiece.h"
#include "ChessPieceType.h"

#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Scene/GeometryComponent.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

ChessPiece::ChessPiece(
	Scene* scene, 
	ChessPieceType* type,
	Player* owner,
	const glm::ivec2& startPos)
	: _scene(scene)
	, _type(type)
	, _originalType(type)
	, _owner(owner)
	, _position(startPos)
	, _startPositon(startPos)
	, _isAlive(false)
	, _entity(Scene::INVALID_ENTITY)
{
	_entity = scene->CreateEntity();

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

const glm::ivec2& ChessPiece::GetPositon() const
{
	return _position;
}

void ChessPiece::SetPosition(const glm::ivec2& pos)
{
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

void ChessPiece::Kill()
{
	_isAlive = true;
}

void ChessPiece::Reset()
{
	_isAlive = true;
	SetPosition(_startPositon);

	_type = _originalType;

	auto geomComp = _scene->GetComponent<GeometryComponent>(_entity);
	geomComp->model = _type->GetModle(_owner);
}

void ChessPiece::UpdatePossibleMoves(ChessBoard* board)
{
	_possibleMoves.clear();
	_type->GetMoveSet().GetAvailableMoves(this, board, _possibleMoves);
}

const std::vector<ChessPieceMoveSet::Move>& ChessPiece::GetPossibleMoves() const
{
	return _possibleMoves;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
