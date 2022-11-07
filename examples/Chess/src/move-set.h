#ifndef BEMBEL_CHESSPIECEMOVESET_H
#define BEMBEL_CHESSPIECEMOVESET_H
#include <memory>
#include <vector>

#include "../chess-game.h"

// clang-format off
import <glm/gtc/quaternion.hpp>;
// clang-format on

class ChessPiece;
class MoveSet {
  public:
    class MoveTemplate {
      public:
        MoveTemplate(){};
        virtual ~MoveTemplate(){};

        virtual void getPosibleMoveParameter(ChessPiece*, const ChessBoard&, std::vector<int>&) = 0;
        virtual glm::vec2 getTargetPosition(ChessPiece*, int)                                   = 0;

        virtual void startMove(ChessPiece*, ChessBoard&, int) = 0;
        virtual void endeMove(ChessPiece*, ChessBoard&, int)  = 0;

        virtual bool updateMoveAnimation(double, ChessPiece*, int) = 0;
    };

    struct Move {
        MoveTemplate* move;
        int           param;
    };

  public:
    MoveSet();
    ~MoveSet();

    void addMove(const glm::ivec2& dir, unsigned maxDist, bool attack = true, bool move = true);
    void addMove(const glm::ivec2& dir, bool attack = true, bool move = true);

    void addMoveTemplate(std::shared_ptr<MoveTemplate>);

    void getAvailableMoves(ChessPiece*, const ChessBoard&, std::vector<Move>&);

  private:
    std::vector<std::shared_ptr<MoveTemplate>> moves;
};
#endif // include guards
