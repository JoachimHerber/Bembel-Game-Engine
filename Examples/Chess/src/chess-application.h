
#ifndef BEMBEL_CHESSAPPLICATION_H
#define BEMBEL_CHESSAPPLICATION_H

#include <bembel/bembel.hpp>

#include "camera-controle.h"
#include "chess/chess-game.h"

class ChessApplication : public bembel::kernel::Application {
 public:
  ChessApplication();
  ~ChessApplication();

  virtual bool init() override;
  virtual void cleanup() override;

  virtual void update(double time) override;

  void handleEvent(const bembel::kernel::WindowShouldCloseEvent&);
  void handleEvent(const bembel::kernel::FrameBufferResizeEvent&);

 private:
  bembel::graphics::GraphicSystem* graphic_system;

  std::unique_ptr<ChessGame> chess_game;

  std::shared_ptr<CameraControle> camera;
};
#endif // include guards
