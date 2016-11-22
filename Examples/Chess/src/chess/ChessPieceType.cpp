/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "ChessPieceType.h"

#include <BembelKernel/Rendering/GeometryModel.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

ChessPieceType::ChessPieceType()
{}

ChessPieceType::~ChessPieceType()
{}

void ChessPieceType::SetModle(Player* player, AssetHandle model)
{
	_modle[player] = model;
}

void ChessPieceType::SetModle(
	Player* player,
	AssetManager* assetMgr, 
	const std::string& model)
{
	SetModle(player, assetMgr->GetAssetHandle<GeometryModel>(model));
}

AssetHandle ChessPieceType::GetModle(Player* player)
{
	auto it = _modle.find(player);
	if (it != _modle.end())
		return it->second;

	return AssetHandle();
}

void ChessPieceType::AddMove(const glm::ivec2& dir, unsigned maxDist, bool attack /*= true*/, bool move /*= true*/)
{
	_moveSet.AddMove(dir, maxDist, attack, move);
}

void ChessPieceType::AddMove(const glm::ivec2& dir, bool attack /*= true*/, bool move /*= true*/)
{
	_moveSet.AddMove(dir, attack, move);
}

ChessPieceMoveSet& ChessPieceType::GetMoveSet()
{
	return _moveSet;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
