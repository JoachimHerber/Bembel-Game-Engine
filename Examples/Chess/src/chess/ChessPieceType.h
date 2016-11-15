#ifndef BEMBEL_CHESSPIECETYPE_H
#define BEMBEL_CHESSPIECETYPE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelKernel/Assets/AssetHandle.h>
#include <BembelKernel/Assets/AssetManager.h>

#include <memory>
#include <map>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Player;

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

private:
	std::map<Player*, AssetHandle> _modle;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
