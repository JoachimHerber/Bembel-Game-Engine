/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "ChessPieceType.h"

#include <BembelGraphics/Geometry/GeometryModel.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

ChessPieceType::ChessPieceType()
{}

ChessPieceType::~ChessPieceType()
{}

std::array<AssetHandle, 2>& ChessPieceType::GetModles()
{
	return _modles;
}

const std::array<AssetHandle, 2>& ChessPieceType::GetModles() const
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

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
