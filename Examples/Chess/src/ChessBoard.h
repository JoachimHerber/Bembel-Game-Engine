#ifndef BEMBEL_CHESSBOARD_H
#define BEMBEL_CHESSBOARD_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelKernel/Scene/Scene.h>

#include <memory>
#include <vector>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class ChessBoard
{
public:
	ChessBoard();
	~ChessBoard();

	void Init(std::shared_ptr<Scene>, unsigned w, unsigned h);

private:
	std::shared_ptr<Scene> _scene;

	std::vector<std::vector<Scene::EntityID>> _chessPices;
	std::vector<std::vector<Scene::EntityID>> _tiles;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
