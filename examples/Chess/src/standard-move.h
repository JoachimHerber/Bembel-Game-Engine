#ifndef BEMBEL_STANDARDMOVE_H
#define BEMBEL_STANDARDMOVE_H

// clang-format off
import <glm/glm.hpp>;
// clang-format on

#include "move-set.h"

class StandardMove : public MoveSet::MoveTemplate {
  public:
    StandardMove(const glm::ivec2& dir, unsigned maxDist, bool attack = true, bool move = true);
    StandardMove(const glm::ivec2& dir, bool attack = true, bool move = true);
    ~StandardMove();

    virtual void getPosibleMoveParameter(
        ChessPiece*, const ChessBoard&, std::vector<int>&) override;
    virtual glm::vec2 getTargetPosition(ChessPiece*, int) override;

    virtual void startMove(ChessPiece*, ChessBoard&, int) override;
    virtual void endeMove(ChessPiece*, ChessBoard&, int) override;

    virtual bool updateMoveAnimation(double, ChessPiece*, int) override;

  private:
    glm::ivec2 direction;
    unsigned   max_distance;
    bool       attack;
    bool       move;
};
#endif // include guards
