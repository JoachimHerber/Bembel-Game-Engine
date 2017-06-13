#ifndef BEMBEL_CHESSPIECETYPE_H
#define BEMBEL_CHESSPIECETYPE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <bembel-kernel/assets/asset-handle.h>
#include <bembel-kernel/assets/asset-manager.h>

#include "moves/move-set.h"

#include <memory>
#include <array>
#include <map>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class ChessPieceType
{
public:
	ChessPieceType();
	~ChessPieceType();

	std::array<bembel::AssetHandle, 2>& GetModles();
	const std::array<bembel::AssetHandle, 2>& GetModles() const;

	void AddMove(const glm::ivec2& dir,
				 unsigned maxDist,
				 bool attack = true,
				 bool move = true);
	void AddMove(const glm::ivec2& dir,
				 bool attack = true,
				 bool move = true);
	MoveSet& GetMoveSet();

private:
	std::array<bembel::AssetHandle, 2> _modles;
	
	MoveSet _moveSet;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
