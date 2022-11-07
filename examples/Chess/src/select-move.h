#ifndef BEMBEL_SELECTMOVESTATE_H
#define BEMBEL_SELECTMOVESTATE_H
#include "game-state.h"

class SelectionPointer;
class ChessPiece;
class PerformMoveState;

class SelectMoveState : public GameState {
 public:
  SelectMoveState(ChessGame* game, SelectionPointer*);
  ~SelectMoveState();

  void setPerformMoveState(PerformMoveState*);

  void setChessPiece(ChessPiece*);

  virtual void onEnterState();
  virtual void onExitState();

  virtual void update(double time);

  void selectedMove();

 private:
  void updateSelection(int i);

 private:
  SelectionPointer* pointer;
  ChessPiece* chess_piece = nullptr;

  int selcted_move = -1;

  PerformMoveState* perform_move;
};
#endif // include guards
