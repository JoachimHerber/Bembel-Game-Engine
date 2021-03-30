#include "move-set.h"

#include "standard-move.h"

MoveSet::MoveSet() {
}

MoveSet::~MoveSet() {
}

void MoveSet::addMove(
  const glm::ivec2& dir,
  unsigned maxDist,
  bool attack /*= true*/,
  bool move /*= true*/) {
  this->addMoveTemplate(
    std::make_shared<StandardMove>(dir, maxDist, attack, move));
}

void MoveSet::addMove(
  const glm::ivec2& dir, bool attack /*= true*/, bool move /*= true*/) {
  this->addMoveTemplate(std::make_shared<StandardMove>(dir, attack, move));
}

void MoveSet::addMoveTemplate(std::shared_ptr<MoveTemplate> move) {
  this->moves.push_back(move);
}

void MoveSet::getAvailableMoves(
  ChessPiece* chessPice, const ChessBoard& board, std::vector<Move>& moves) {
  for(auto& move : this->moves) {
    std::vector<int> params;
    move->getPosibleMoveParameter(chessPice, board, params);

    for(const auto& it : params) { moves.push_back(Move{move.get(), it}); }
  }
}
