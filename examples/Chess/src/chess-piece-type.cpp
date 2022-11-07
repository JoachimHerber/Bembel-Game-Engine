#include "chess-piece-type.h"

using namespace bembel::kernel;

ChessPieceType::ChessPieceType() {
}

ChessPieceType::~ChessPieceType() {
}

std::array<AssetHandle, 2>& ChessPieceType::getModles() {
  return this->modles;
}

const std::array<AssetHandle, 2>& ChessPieceType::getModles() const {
  return this->modles;
}

void ChessPieceType::addMove(
  const glm::ivec2& dir,
  unsigned maxDist,
  bool attack /*= true*/,
  bool move /*= true*/) {
  this->move_set.addMove(dir, maxDist, attack, move);
}

void ChessPieceType::addMove(
  const glm::ivec2& dir, bool attack /*= true*/, bool move /*= true*/) {
  this->move_set.addMove(dir, attack, move);
}

MoveSet& ChessPieceType::getMoveSet() {
  return this->move_set;
}
