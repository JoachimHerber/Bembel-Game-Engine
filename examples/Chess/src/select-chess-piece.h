#ifndef BEMBEL_SELETCHESSPIECESTATE_H
#define BEMBEL_SELETCHESSPIECESTATE_H

#include "game-state.h"

class SelectionPointer;
class ChessPiece;
class SelectMoveState;

class SelectChessPieceState : public GameState {
 public:
  SelectChessPieceState(ChessGame* game, unsigned player, SelectionPointer*);
  ~SelectChessPieceState();

  void setSelectMoveState(SelectMoveState*);

  virtual void onEnterState();
  virtual void onExitState();

  virtual void update(double time);

  void selectChessPiece();

 private:
  void setSelectedChessPice(ChessPiece*);

 private:
  unsigned player;

  SelectionPointer* pointer;
  ChessPiece* selected_chess_piece = nullptr;

  SelectMoveState* select_move;
};
#endif // include guards
