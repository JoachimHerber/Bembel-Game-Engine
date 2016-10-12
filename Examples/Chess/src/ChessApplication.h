#ifndef BEMBEL_CHESSAPPLICATION_H
#define BEMBEL_CHESSAPPLICATION_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include <BembelKernel/Application.h>
#include <BembelKernel/Events/EventManager.h>
#include <BembelKernel/Assets/AssetManager.h>
#include <BembelKernel/Scene/Scene.h>
#include <BembelGraphics/GraphicSystem.h>
#include <BembelGraphics/RenderingPipeline/RenderingPipeline.h>
#include "ChessBoard.h"
#include "CameraControle.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class ChessApplication : public Application
{
public:
	ChessApplication();
	~ChessApplication();

	virtual bool Init() override;
	virtual void Cleanup() override;

	virtual void Update(double time) override;

	void HandleEvent(const WindowShouldCloseEvent&);

private:
	bool InitAssets();
	bool InitScene();

private:
	std::shared_ptr<GraphicSystem>  _graphicSys;

	std::shared_ptr<AssetManager> _assetMgr;

	std::shared_ptr<Scene> _scene;

	std::shared_ptr<ChessBoard>     _board;
	std::shared_ptr<CameraControle> _cam;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
