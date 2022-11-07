#include "initial-pawn-move.h"

#include <bembel/bembel.hpp>

#include "../chess-piece.h"
#include "../player.h"

using namespace bembel::kernel;

InitialPawnMove::InitialPawnMove() {
}

InitialPawnMove::~InitialPawnMove() {
}

void InitialPawnMove::getPosibleMoveParameter(
  ChessPiece* piece, const ChessBoard& board, std::vector<int>& params) {
  if(piece->hasMoved()) return;

  unsigned owner = piece->getOwner();
  auto p1        = piece->getPositon() + glm::ivec2{owner == 0 ? 1 : -1, 0};
  auto p2        = piece->getPositon() + glm::ivec2{owner == 0 ? 2 : -2, 0};

  if(board[p1.x][p1.y] != nullptr || board[p2.x][p2.y] != nullptr) { return; }
  params.push_back(0);
}

glm::vec2 InitialPawnMove::getTargetPosition(ChessPiece* piece, int) {
  unsigned owner = piece->getOwner();
  return piece->getPositon() + glm::ivec2{owner == 0 ? 2 : -2, 0};
}

void InitialPawnMove::startMove(ChessPiece*, ChessBoard&, int) {
}

void InitialPawnMove::endeMove(ChessPiece* piece, ChessBoard& board, int) {
  glm::vec2 pos       = piece->getPositon();
  board[pos.x][pos.y] = nullptr;

  glm::vec2 targetPos = this->getTargetPosition(piece, 0);
  piece->setPosition(targetPos);
  board[targetPos.x][targetPos.y] = piece;
}

bool InitialPawnMove::updateMoveAnimation(double time, ChessPiece* piece, int) {
  auto scene = piece->getScene();

  glm::ivec2 pos = 2 * piece->getPositon();
  glm::ivec2 dir = 2 * glm::ivec2{piece->getOwner() == 0 ? 2 : -2, 0};

  float progress = time / 2.0f;
  float fract    = progress - std::floor(progress);
  if(progress > 1) return true; // animation finished

  auto& entitiyPos =
    scene->getComponent<PositionComponent>(piece->getEntity())->position;
  entitiyPos.x = pos.x + float(progress) * dir.x;
  entitiyPos.z = pos.y + float(progress) * dir.y;
  entitiyPos.y = 4 * (fract - fract * fract);

  return false;
}
