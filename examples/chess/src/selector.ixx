module;
#include <chrono>
#include <optional>
export module bembel.examples.chess:Selector;

import bembel;
import :Board;
import :Moves;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export class Selector {
  public:
    Selector(Scene* scene, Camera* camera);
    ~Selector();

    void handleEvent(CursorMovedEvent const& event);

  private:
    struct Ray {
        vec3 origin;
        vec3 direction;
    };
    Ray calculateRay(In<vec2> cursor_pos) const;

    void updateSelection(In<Ray> ray);

    virtual void onSelect(EntityID id) = 0;

  protected:
    Scene*  m_scene;
    Camera* m_camera;
};

export class ChessPieceSelector : Selector {
  public:
    ChessPieceSelector(ChessBoard* board, ChessPlayer player, Camera* camera)
      : Selector{board->getScene(), camera}, m_board{board}, m_player{player} {}
    ~ChessPieceSelector() {}

    ChessPieceEntity getSelectedChessPiece() const { return m_selection; }

  private:
    virtual void onSelect(EntityID id);

  private:
    ChessBoard*      m_board;
    ChessPlayer      m_player;
    ChessPieceEntity m_selection;
};

export class MoveSelector : Selector {
  public:
    MoveSelector(ChessBoard* board, ChessPieceEntity chess_piece, Camera* camera);
    ~MoveSelector() {}

    std::optional<Move> getSelectedMove() const { return m_selection; }

  private:
    virtual void onSelect(EntityID id);

  private:
    ChessBoard*         m_board;
    ChessPieceEntity    m_chess_piece;
    std::vector<Move>   m_possible_moves;
    std::optional<Move> m_selection;
};

} // namespace bembel::examples::chess
