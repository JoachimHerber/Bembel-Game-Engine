
#include "player.h"

#include "chess-game.h"
#include "chess-piece.h"

using namespace bembel::kernel;

Player::Player(ChessGame* game, const std::string& name)
: game(game)
, name(name) {
}

ChessGame* Player::getChessGame() const {
  return this->game;
}

const std::string& Player::getName() const {
  return this->name;
}

const std::vector<ChessPiece*>& Player::getChessPieces() const {
  return this->chess_pices;
}

void Player::clearChessPieces() {
  this->chess_pices.clear();
}

void Player::addChessPiece(ChessPiece* piece) {
  this->chess_pices.push_back(piece);
}

void Player::removeChessPiece(ChessPiece* piece) {
  for(size_t n = 0; n < this->chess_pices.size(); ++n) {
    if(this->chess_pices[n] == piece) {
      this->chess_pices.erase(this->chess_pices.begin() + n);
      return;
    }
  }
}

void Player::clearCaptureChessPieces() {
  this->captured_pices.clear();
}

void Player::captureChessPiece(ChessPiece* piece) {
  unsigned row = this->captured_pices.size() / this->capture_area_pices_per_row;
  unsigned col = this->captured_pices.size() % this->capture_area_pices_per_row;

  this->captured_pices.push_back(piece);

  auto scene = this->game->getScene();
  auto& entitiyPos =
    scene->getComponent<PositionComponent>(piece->getEntity())->position;

  entitiyPos = this->capture_area_pos;
  entitiyPos += this->capture_area_row_offset * float(row);
  entitiyPos += this->capture_area_collum_offset * float(col);
}
