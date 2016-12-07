/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "StandardMove.h"

#include "../ChessPiece.h"
#include "../ChessBoard.h"
#include "../Player.h"

#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/PositionComponent.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

StandardMove::StandardMove(
	const glm::ivec2& dir,
	unsigned maxDist,
	bool attack, 
	bool move)
	: _direction(dir)
	, _maxDistance(maxDist)
	, _attack(attack)
	, _move(move)
{}
StandardMove::StandardMove(
	const glm::ivec2& dir, bool attack, bool move)
	: _direction(dir)
	, _maxDistance(~0U)
	, _attack(attack)
	, _move(move)
{}

StandardMove::~StandardMove()
{}

void StandardMove::GetPosibleMoveParameter(
	ChessPiece* piece,
	std::vector<int>& params)
{
	Player* owner = piece->GetOwner();

	for (unsigned u = 1; u <= _maxDistance; ++u)
	{
		glm::ivec2 target = GetTargetPosition(piece, u);

		if (!piece->GetBoard()->IsPositionValid(target))
			return;

		ChessPiece* other = piece->GetBoard()->GetChessPieceAt(target);
		if (other != nullptr)
		{
			if (_attack && other->GetOwner() != owner)
				params.push_back(u);
			return;
		}
		else if (_move)
		{
			params.push_back(u);
		}
	}
}

glm::vec2 StandardMove::GetTargetPosition(
	ChessPiece* piece, int dist)
{
	Player* owner = piece->GetOwner();
	return piece->GetPositon() + 
		dist*owner->RotateOffset(_direction);
}

bool StandardMove::UpdateMoveAnimation(
	double time, 
	ChessPiece* piece,
	int param)
{
	auto scene = piece->GetBoard()->GetScene();

	glm::ivec2 pos = 2*piece->GetPositon();
	glm::ivec2 dir = 2*piece->GetOwner()->RotateOffset(_direction);

	float dist = std::sqrt(float(_direction.x*_direction.x + _direction.y*_direction.y));
	float progress = time / dist;
	float fract = progress-std::floor(progress);
	if (progress > param)
		return true; // animation finished

	auto& entitiyPos = scene->GetComponent<PositionComponent>(
		piece->GetEntity())->position;
	entitiyPos.x = pos.x + float(progress)*dir.x;
	entitiyPos.z = pos.y + float(progress)*dir.y;
	entitiyPos.y = 4*dist*(fract-fract*fract);

	return false;
}

void StandardMove::StartMove(
	ChessPiece* piece, int param)
{
	glm::vec2 targetPos = GetTargetPosition(piece, param);
	auto other = piece->GetBoard()->GetChessPieceAt(targetPos);
	if (other)
	{
		other->Kill(piece->GetOwner());
	}
}

void StandardMove::EndeMove(
	ChessPiece* piece, int param)
{
	glm::vec2 targetPos = GetTargetPosition(piece, param);
	piece->SetPosition(targetPos);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
