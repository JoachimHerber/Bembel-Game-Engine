#ifndef BEMBEL_CHESSPIECETYPE_H
#define BEMBEL_CHESSPIECETYPE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelKernel/Assets/AssetHandle.h>
#include <BembelKernel/Assets/AssetManager.h>

#include "Moves/MoveSet.h"

#include <memory>
#include <array>
#include <map>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class MoveSet;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class ChessPieceType
{
public:
	ChessPieceType();
	~ChessPieceType();

	std::array<AssetHandle, 2>& GetModles();
	const std::array<AssetHandle, 2>& GetModles() const;

	void AddMove(const glm::ivec2& dir,
				 unsigned maxDist,
				 bool attack = true,
				 bool move = true);
	void AddMove(const glm::ivec2& dir,
				 bool attack = true,
				 bool move = true);
	MoveSet& GetMoveSet();

private:
	std::array<AssetHandle, 2> _modles;
	
	MoveSet _moveSet;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
