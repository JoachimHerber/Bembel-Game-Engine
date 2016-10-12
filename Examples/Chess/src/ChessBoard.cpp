/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "ChessBoard.h"

#include <BembelKernel/Assets/AssetManager.h>
#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Renderig/GeometryModel.h>

#include <BembelGraphics/Geometry/GeometryInstance.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

ChessBoard::ChessBoard()
{}

ChessBoard::~ChessBoard()
{}

void ChessBoard::Init(
	std::shared_ptr<Scene> scene, 
	unsigned w, unsigned h)
{
	_scene = scene;

	auto assetMgr = _scene->GetAssetManager();

	AssetHandle whiteTile = 
		assetMgr->GetAssetHandle<GeometryModel>("white.tile");
	AssetHandle blackTile =
		assetMgr->GetAssetHandle<GeometryModel>("black.tile");

	_tiles.resize(w);
	for (unsigned u = 0; u < w; ++u)
	{
		_tiles[u].resize(h, ~size_t(0));
		for (unsigned v = 0; v < h; ++v)
		{
			_tiles[u][v] = _scene->CreateEntity();
			auto posComp = _scene->CreateComponent<PositionComponent>(_tiles[u][v]);
			posComp->position = glm::vec3(2.0f*u-w, 0, 2.0f*v-h);
			auto geomComt = _scene->CreateComponent<GeometryInstance>(_tiles[u][v]);
			geomComt->isCastingShadow = false;
			geomComt->model = ((u+v)%2 == 0 ? whiteTile : blackTile);
		}
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
