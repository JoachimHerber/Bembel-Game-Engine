/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "StandardMove.h"

#include "../ChessPiece.h"
#include "../ChessBoard.h"
#include "../Player.h"

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

void StandardMove::GetAvailableTargetPositions(
	ChessPiece* piece,
	ChessBoard* board,
	std::vector<glm::ivec2>& targets)
{
	glm::ivec2 curentPos = piece->GetPositon();
	Player* owner = piece->GetOwner();

	glm::ivec2 pos = curentPos + owner->RotateOffset(_direction);
	for (unsigned u = 0; u < _maxDistance; ++u)
	{
		if (!board->IsPositionValid(pos))
			return;

		ChessPiece* other = board->GetChessPieceAt(pos);
		if (other != nullptr)
		{
			if (_attack && other->GetOwner() != owner)
				targets.push_back(pos);

			return;
		}
		else if (_move)
			targets.push_back(pos);

		pos += owner->RotateOffset(_direction);
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
