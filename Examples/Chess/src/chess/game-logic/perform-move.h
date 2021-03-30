#ifndef BEMBEL_PERFORMMOVESTATE_H
#define BEMBEL_PERFORMMOVESTATE_H

#include <vector>

#include "../moves/move-set.h"
#include "game-state.h"

class ChessPiece;

class PerformMoveState : public GameState {
 public:
  PerformMoveState(ChessGame*);
  ~PerformMoveState();

  using Move = MoveSet::Move;

  void init(GameState*);

  virtual void onEnterState() override;
  virtual void onExitState() override;

  virtual void update(double time) override;

  void setMove(ChessPiece*, const Move&);

 private:
  ChessPiece* chess_piece;
  Move move;

  GameState* next_state = nullptr;

  double time;
};
#endif // include guards
