#ifndef BEMBEL_CHESSPIECETYPE_H
#define BEMBEL_CHESSPIECETYPE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelKernel/Assets/AssetHandle.h>
#include <BembelKernel/Assets/AssetManager.h>

#include "Moves/MoveSet.h"

#include <memory>
#include <map>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Player;
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

	void SetModle(Player*, AssetHandle);
	void SetModle(Player*, AssetManager*, const std::string&);

	AssetHandle GetModle(Player*);

	void AddMove(const glm::ivec2& dir,
				 unsigned maxDist,
				 bool attack = true,
				 bool move = true);
	void AddMove(const glm::ivec2& dir,
				 bool attack = true,
				 bool move = true);
	MoveSet& GetMoveSet();

private:
	std::map<Player*, AssetHandle> _modle;
	
	MoveSet _moveSet;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
