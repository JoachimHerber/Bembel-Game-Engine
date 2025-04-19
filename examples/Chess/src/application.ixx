export module bembel.examples.chess;

import std;
import bembel;
import :Board;
import :Camera;
import :Logic;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export class Application
  : public kernel::Application<GraphicSystem, PhysicsSystem, ParticleSystem, GuiSystem> {
  public:
    Application();
    ~Application();

    virtual bool init(std::span<std::string_view> args) override;
    virtual void cleanup() override;

    void handleEvent(In<WindowShouldCloseEvent>);
    void handleEvent(In<FrameBufferResizeEvent>);
    void handleEvent(In<KeyPressEvent>);

  private:
    EventHandlerGuard<WindowShouldCloseEvent, FrameBufferResizeEvent, KeyPressEvent> m_guard{this};

    std::shared_ptr<Scene>      m_scene;
    std::unique_ptr<ChessBoard> m_chess_board;
    coro::Task<>                m_game_logic;

    std::unique_ptr<CameraControle> m_camera;
};
} // namespace bembel::examples::chess
