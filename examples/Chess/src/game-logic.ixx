module;
#include "bembel/pch.h"
export module bembel.examples.chess.logic;

import bembel;
import bembel.examples.chess.board;
import bembel.examples.chess.camera;
import bembel.examples.chess.selection;
import bembel.examples.chess.move_animations;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export class GameLogic {
  public:
    GameLogic(
        ChessBoard*             board,
        SelectionPointer*       selection_ptr,
        CameraControle*         camera,
        GraphicalUserInterface* gui
    );

    ChessBoard*             getChessBoard() { return m_board; }
    SelectionPointer*       getSelectionPointer() { return m_selection_ptr; }
    CameraControle*         getCameraControle() { return m_camera; }
    GraphicalUserInterface* getGui() { return m_gui; }

    struct Move {
        enum MoveType {
            MOVE,
            CAPUTE,
            CAPUTE_EN_PASSANT,
            CASTLING,
        };
        MoveType type;
        ivec2    to;
    };

    std::vector<Move> getPossibleMoves(ChessPiece pice);

    void update(double time);
    void onClick();

    class GameState {
      public:
        virtual ~GameState() = default;

        virtual void update(double time){};

        virtual void onClick(){};
        virtual void onEnter(){};
        virtual void onExit(){};
    };

    void setState(std::unique_ptr<GameState> state);

    class SelectChessPice : public GameState {
      public:
        SelectChessPice(GameLogic*, ChessPlayer player);

        virtual void update(double time) override;

        virtual void onClick() override;
        virtual void onEnter() override;

      private:
        GameLogic*  m_logic;
        ChessPlayer m_player;

        ChessPiece m_selection;
    };

    class SelectMove : public GameState {
      public:
        SelectMove(GameLogic* logic, ChessPiece pice);

        virtual void update(double time) override;

        virtual void onClick() override;
        virtual void onEnter() override;

      private:
        SelectionHighlightComponent getHighlightComponentOfSelectedTile() {
            if(m_selection >= m_moves.size()) return {nullptr};
            return m_logic->getChessBoard()
                ->getTileAt(m_moves[m_selection].to)
                .getComponent<SelectionHighlightComponent>();
        }

      private:
        GameLogic* m_logic;
        ChessPiece m_pice;

        std::vector<Move> m_moves;
        std::size_t       m_selection = ~0;
    };

    class PerformMove : public GameState {
      public:
        PerformMove(GameLogic* logic, ChessPiece pice, Move move);

        virtual void update(double time) override;

        virtual void onEnter() override;

      private:
        void FinishMove();

      private:
        GameLogic*                     m_logic;
        ChessPiece                     m_pice;
        Move                           m_move;
        std::unique_ptr<MoveAnimation> m_animation;
    };

  private:
    void getPossiblePawnMoves(ChessPiece pice, std::vector<Move>&);
    void getPossibleKnightMoves(ChessPiece pice, std::vector<Move>&);
    void getPossibleRookMoves(ChessPiece pice, std::vector<Move>&);
    void getPossibleBishopMoves(ChessPiece pice, std::vector<Move>&);
    void getPossibleKingMoves(ChessPiece pice, std::vector<Move>&);

    bool addMove(ChessPiece pice, std::vector<Move>&, ivec2 pos, bool can_capture);

  private:
    ChessBoard*             m_board;
    SelectionPointer*       m_selection_ptr;
    CameraControle*         m_camera;
    GraphicalUserInterface* m_gui;

    std::unique_ptr<GameState> m_state;
    std::unique_ptr<GameState> m_last_state;
};

} // namespace bembel::examples::chess