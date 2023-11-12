module;
#include <memory>
#include <span>
#include <string_view>
export module bembel.examples.chess;

import bembel;
import :Board;
import :Camera;
import :Logic;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export class Application : public kernel::Application {
  public:
    Application();
    ~Application();

    virtual bool init(std::span<std::string_view> args) override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void handleEvent(In<WindowShouldCloseEvent>);
    void handleEvent(In<FrameBufferResizeEvent>);
    void handleEvent(In<KeyPressEvent>);

  private:
    GraphicSystem* m_graphic_system;
    PhysicsSystem* m_physics_system;
    GuiSystem*     m_gui_system;

    std::shared_ptr<Scene>      m_scene;
    std::unique_ptr<ChessBoard> m_chess_board;
    GameLogicCoroutine          m_game_logic;

    std::unique_ptr<CameraControle> m_camera;

    Signal<> m_frame_sync;
};
} // namespace bembel::examples::chess
