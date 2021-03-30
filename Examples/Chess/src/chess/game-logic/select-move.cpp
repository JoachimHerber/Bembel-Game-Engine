
#include "select-move.h"

#include <iostream>

#include "../../selection-component.h"
#include "../chess-game.h"
#include "../chess-piece.h"
#include "perform-move.h"
#include "selection-pointer.h"

SelectMoveState::SelectMoveState(ChessGame* game, SelectionPointer* pointer)
: GameState(game)
, pointer(pointer) {
}

SelectMoveState::~SelectMoveState() {
}

void SelectMoveState::setPerformMoveState(PerformMoveState* state) {
  this->perform_move = state;
}

void SelectMoveState::setChessPiece(ChessPiece* piece) {
  this->chess_piece = piece;
}

void SelectMoveState::onEnterState() {
  if(this->chess_piece) {
    auto entity = this->chess_piece->getEntity();
    auto selectComp =
      this->game->getScene()->getComponent<SelectionComponent>(entity);
    selectComp->state = SelectionComponent::State::SELECTED;
  }
  this->pointer->getSelectSignal().addSlot(
    this, &SelectMoveState::selectedMove);
}

void SelectMoveState::onExitState() {
  if(this->chess_piece) {
    auto entity = this->chess_piece->getEntity();
    auto selectComp =
      this->game->getScene()->getComponent<SelectionComponent>(entity);
    selectComp->state = SelectionComponent::State::UNSELECTABLE;

    for(auto& it : this->chess_piece->getPossibleMoves()) {
      glm::ivec2 target =
        it.move->getTargetPosition(this->chess_piece, it.param);

      selectComp        = this->game->getBoardTileSelectionComponent(target);
      selectComp->state = SelectionComponent::State::UNSELECTABLE;
    }
  }
  this->pointer->getSelectSignal().removeSlot(
    this, &SelectMoveState::selectedMove);
}

void SelectMoveState::update(double time) {
  glm::ivec2 tile = this->pointer->getSelectedTile();

  if(tile.x >= 0 && tile.x < 8 && tile.y >= 0 && tile.y < 8) {
    for(size_t n = 0; n < this->chess_piece->getPossibleMoves().size(); ++n) {
      auto& it = this->chess_piece->getPossibleMoves()[n];
      glm::ivec2 target =
        it.move->getTargetPosition(this->chess_piece, it.param);

      if(target == tile) {
        this->updateSelection(n);
        return;
      }
    }
  }
  this->updateSelection(-1);
}

void SelectMoveState::selectedMove() {
  if(this->selcted_move >= 0) {
    this->perform_move->setMove(
      this->chess_piece,
      this->chess_piece->getPossibleMoves()[this->selcted_move]);
    this->nextState(this->perform_move);
    this->selcted_move = -1;
  }
}

void SelectMoveState::updateSelection(int i) {
  if(this->selcted_move >= 0) {
    auto& move = this->chess_piece->getPossibleMoves()[this->selcted_move];
    glm::ivec2 target =
      move.move->getTargetPosition(this->chess_piece, move.param);

    auto selectComp   = this->game->getBoardTileSelectionComponent(target);
    selectComp->state = SelectionComponent::State::SELECTABLE;
  }
  this->selcted_move = i;
  if(this->selcted_move >= 0) {
    auto& move = this->chess_piece->getPossibleMoves()[this->selcted_move];
    glm::ivec2 target =
      move.move->getTargetPosition(this->chess_piece, move.param);

    auto selectComp   = this->game->getBoardTileSelectionComponent(target);
    selectComp->state = SelectionComponent::State::FOCUSED;
  }
}
