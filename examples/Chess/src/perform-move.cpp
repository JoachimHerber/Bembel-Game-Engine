#include "perform-move.h"

#include <bembel/bembel.hpp>

#include "../chess-game.h"
#include "../chess-piece.h"
#include "select-chess-piece.h"

PerformMoveState::PerformMoveState(ChessGame* game)
: GameState(game)
, chess_piece(nullptr)
, move({nullptr, 0}) {
}

PerformMoveState::~PerformMoveState() {
}

void PerformMoveState::init(GameState* state) {
  this->next_state = state;
}

void PerformMoveState::onEnterState() {
  this->move.move->startMove(
    this->chess_piece, this->game->getChessBoard(), this->move.param);
  this->time = 0;
}

void PerformMoveState::onExitState() {
  this->move.move->endeMove(
    this->chess_piece, this->game->getChessBoard(), this->move.param);
}

void PerformMoveState::update(double time) {
  this->time += time;
  if(this->move.move->updateMoveAnimation(
       this->time, this->chess_piece, this->move.param)) {
    this->nextState(this->next_state);
  }
}

void PerformMoveState::setMove(ChessPiece* chessPiece, const Move& move) {
  this->chess_piece = chessPiece;
  this->move        = move;
}
