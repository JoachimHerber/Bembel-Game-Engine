/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "StandardMove.h"

#include "../ChessPiece.h"
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
	const ChessBoard& board,
	std::vector<int>& params)
{
	unsigned owner = piece->GetOwner();

	for (unsigned u = 1; u <= _maxDistance; ++u)
	{
		glm::ivec2 target = GetTargetPosition(piece, u);

		if (target.x < 0 || target.y < 0 || target.x >= 8 || target.y >= 8 )
			return;

		ChessPiece* other = board[target.x][target.y];
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
	glm::ivec2 dir = _direction;
	if( piece->GetOwner() == 1 )
		dir.x *= -1;
	return piece->GetPositon() + dist*dir;
}

bool StandardMove::UpdateMoveAnimation(
	double time, 
	ChessPiece* piece,
	int param)
{
	auto scene = piece->GetScene();

	glm::ivec2 pos = 2*piece->GetPositon();
	glm::ivec2 dir = 2*_direction;
	if( piece->GetOwner() == 1 )
		dir.x *= -1;

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
	ChessPiece* piece,
	ChessBoard& board,
	int param)
{
	glm::vec2 targetPos = GetTargetPosition(piece, param);
	auto other = board[targetPos.x][targetPos.y];
	if (other)
		other->Kill();
}

void StandardMove::EndeMove(
	ChessPiece* piece, 
	ChessBoard& board, 
	int param)
{
	glm::vec2 pos = piece->GetPositon();
	board[pos.x][pos.y] = nullptr;

	glm::vec2 targetPos = GetTargetPosition(piece, param);
	piece->SetPosition(targetPos);
	board[targetPos.x][targetPos.y] = piece;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
