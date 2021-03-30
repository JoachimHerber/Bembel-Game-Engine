#include "select-chess-piece.h"

#include <iostream>

#include "../../selection-component.h"
#include "../chess-game.h"
#include "../chess-piece.h"
#include "select-move.h"
#include "selection-pointer.h"

SelectChessPieceState::SelectChessPieceState(
  ChessGame* game, unsigned player, SelectionPointer* pointer)
: GameState(game)
, player(player)
, pointer(pointer) {
}

SelectChessPieceState::~SelectChessPieceState() {
}

void SelectChessPieceState::setSelectMoveState(SelectMoveState* state) {
  this->select_move = state;
}

void SelectChessPieceState::onEnterState() {
  this->game->updatePossibleMoves();
  this->pointer->getSelectSignal().addSlot(
    this, &SelectChessPieceState::selectChessPiece);
}

void SelectChessPieceState::onExitState() {
  this->pointer->getSelectSignal().removeSlot(
    this, &SelectChessPieceState::selectChessPiece);
}

void SelectChessPieceState::update(double time) {
  glm::ivec2 tile = this->pointer->getSelectedTile();

  if(tile.x >= 0 && tile.x < 8 && tile.y >= 0 && tile.y < 8) {
    auto* chess_pice = this->game->getChessBoard()[tile.x][tile.y];
    if(chess_pice && chess_pice->getOwner() == this->player) {
      if(!chess_pice->getPossibleMoves().empty()) {
        this->setSelectedChessPice(chess_pice);
        return;
      }
    }
  }

  this->setSelectedChessPice(nullptr);
}

void SelectChessPieceState::selectChessPiece() {
  if(this->selected_chess_piece) {
    this->select_move->setChessPiece(this->selected_chess_piece);
    this->nextState(this->select_move);
  }
}

void SelectChessPieceState::setSelectedChessPice(ChessPiece* pice) {
  if(this->selected_chess_piece) {
    auto entity = this->selected_chess_piece->getEntity();
    auto selectComp =
      this->game->getScene()->getComponent<SelectionComponent>(entity);
    selectComp->state = SelectionComponent::State::UNSELECTABLE;

    for(auto& it : this->selected_chess_piece->getPossibleMoves()) {
      glm::ivec2 target =
        it.move->getTargetPosition(this->selected_chess_piece, it.param);

      selectComp        = this->game->getBoardTileSelectionComponent(target);
      selectComp->state = SelectionComponent::State::UNSELECTABLE;
    }
  }
  this->selected_chess_piece = pice;
  if(this->selected_chess_piece) {
    auto entity = this->selected_chess_piece->getEntity();
    auto selectComp =
      this->game->getScene()->getComponent<SelectionComponent>(entity);
    selectComp->state = SelectionComponent::State::FOCUSED;

    for(auto& it : this->selected_chess_piece->getPossibleMoves()) {
      glm::ivec2 target =
        it.move->getTargetPosition(this->selected_chess_piece, it.param);

      selectComp        = this->game->getBoardTileSelectionComponent(target);
      selectComp->state = SelectionComponent::State::SELECTABLE;
    }
  }
}
