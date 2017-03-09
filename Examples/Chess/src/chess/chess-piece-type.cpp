/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "chess-piece-type.h"

#include <bembel-graphics/geometry/geometry-model.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
ChessPieceType::ChessPieceType()
{}

ChessPieceType::~ChessPieceType()
{}

std::array<bembel::AssetHandle, 2>& ChessPieceType::GetModles()
{
	return _modles;
}

const std::array<bembel::AssetHandle, 2>& ChessPieceType::GetModles() const
{
	return _modles;
}

void ChessPieceType::AddMove(const glm::ivec2& dir, unsigned maxDist, bool attack /*= true*/, bool move /*= true*/)
{
	_moveSet.AddMove(dir, maxDist, attack, move);
}

void ChessPieceType::AddMove(const glm::ivec2& dir, bool attack /*= true*/, bool move /*= true*/)
{
	_moveSet.AddMove(dir, attack, move);
}

MoveSet& ChessPieceType::GetMoveSet()
{
	return _moveSet;
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
