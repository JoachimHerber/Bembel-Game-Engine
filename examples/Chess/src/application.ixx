module;
#include <memory>
#include <span>
#include <string_view>
export module bembel.examples.chess;

import bembel;
import bembel.examples.chess.board;
import bembel.examples.chess.camera;
import bembel.examples.chess.logic;
import bembel.examples.chess.selection;

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

    void handleEvent(WindowShouldCloseEvent const&);
    void handleEvent(FrameBufferResizeEvent const&);

  private:
    GraphicSystem* m_graphic_system;
    GuiSystem*     m_gui_system;

    std::shared_ptr<Scene>            m_scene;
    std::unique_ptr<SelectionPointer> m_selection_ptr;
    std::unique_ptr<ChessBoard>       m_chess_board;
    std::unique_ptr<GameLogic>        m_game_logic;

    std::unique_ptr<CameraControle> m_camera;
};
} // namespace bembel::examples::chess
