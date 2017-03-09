#ifndef BEMBEL_CHESSAPPLICATION_H
#define BEMBEL_CHESSAPPLICATION_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include <bembel-kernel/application.h>
#include <bembel-kernel/events/event-manager.h>
#include <bembel-kernel/assets/asset-manager.h>
#include <bembel-kernel/scene/scene.h>
#include <bembel-graphics/graphic-system.h>
#include <bembel-interaction/interaction-system.h>

#include "chess/chess-game.h"
#include "camera-controle.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class ChessApplication : public bembel::Application
{
public:
	ChessApplication();
	~ChessApplication();

	virtual bool Init() override;
	virtual void Cleanup() override;

	virtual void Update(double time) override;

	void HandleEvent(const bembel::WindowShouldCloseEvent&);

private:
	bool InitGame();

private:
	std::shared_ptr<bembel::GraphicSystem>     _graphicSys;
	std::shared_ptr<bembel::InteractionSystem> _interactionSys;

	std::unique_ptr<ChessGame> _chessGame;

	std::shared_ptr<CameraControle> _cam;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
