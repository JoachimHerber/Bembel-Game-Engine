
#ifndef BEMBEL_INITIALPAWNMOVE_H
#define BEMBEL_INITIALPAWNMOVE_H
#include "move-set.h"

class InitialPawnMove : public MoveSet::MoveTemplate {
 public:
  InitialPawnMove();
  ~InitialPawnMove();

  virtual void getPosibleMoveParameter(
    ChessPiece*, const ChessBoard&, std::vector<int>&) override;

  virtual glm::vec2 getTargetPosition(ChessPiece*, int) override;

  virtual void startMove(ChessPiece*, ChessBoard&, int) override;
  virtual void endeMove(ChessPiece*, ChessBoard&, int) override;

  virtual bool updateMoveAnimation(double, ChessPiece*, int) override;
};
#endif // include guards
