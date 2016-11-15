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

#include <Bembelnteraction/InteractionSystem.h>

#include "Chess/ChessBoard.h"
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

	void NextClessBoard();

	void HandleEvent(const WindowShouldCloseEvent&);

private:
	bool InitAssets();
	bool InitDefaultChessBoard();
	bool InitFourPlayerChessBoard();

private:
	std::shared_ptr<GraphicSystem>     _graphicSys;
	std::shared_ptr<InteractionSystem> _interactionSys;

	std::shared_ptr<AssetManager> _assetMgr;

	std::vector<std::unique_ptr<ChessBoard>> _chessBoards;
	unsigned _currentChessBoard;

	std::shared_ptr<CameraControle> _cam;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
