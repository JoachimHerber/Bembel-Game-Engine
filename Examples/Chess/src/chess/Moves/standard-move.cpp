#include "standard-move.h"

#include <bembel/bembel.hpp>

#include "../chess-piece.h"
#include "../player.h"

using namespace bembel::kernel;

StandardMove::StandardMove(
  const glm::ivec2& dir, unsigned max_dist, bool attack, bool move)
: direction(dir)
, max_distance(max_dist)
, attack(attack)
, move(move) {
}
StandardMove::StandardMove(const glm::ivec2& dir, bool attack, bool move)
: direction(dir)
, max_distance(~0U)
, attack(attack)
, move(move) {
}

StandardMove::~StandardMove() {
}

void StandardMove::getPosibleMoveParameter(
  ChessPiece* piece, const ChessBoard& board, std::vector<int>& params) {
  unsigned owner = piece->getOwner();

  for(unsigned u = 1; u <= this->max_distance; ++u) {
    glm::ivec2 target = this->getTargetPosition(piece, u);

    if(target.x < 0 || target.y < 0 || target.x >= 8 || target.y >= 8) return;

    ChessPiece* other = board[target.x][target.y];
    if(other != nullptr) {
      if(this->attack && other->getOwner() != owner) params.push_back(u);
      return;
    } else if(this->move) {
      params.push_back(u);
    }
  }
}

glm::vec2 StandardMove::getTargetPosition(ChessPiece* piece, int dist) {
  glm::ivec2 dir = this->direction;
  if(piece->getOwner() == 1) dir.x *= -1;
  return piece->getPositon() + dist * dir;
}

bool StandardMove::updateMoveAnimation(
  double time, ChessPiece* piece, int param) {
  auto scene = piece->getScene();

  glm::ivec2 pos = 2 * piece->getPositon();
  glm::ivec2 dir = 2 * this->direction;
  if(piece->getOwner() == 1) dir.x *= -1;

  float dist     = std::sqrt(float(
    this->direction.x * this->direction.x
    + this->direction.y * this->direction.y));
  float progress = time / dist;
  float fract    = progress - std::floor(progress);
  if(progress > param) return true; // animation finished

  auto& entitiyPos =
    scene->getComponent<PositionComponent>(piece->getEntity())->position;
  entitiyPos.x = pos.x + float(progress) * dir.x;
  entitiyPos.z = pos.y + float(progress) * dir.y;
  entitiyPos.y = 4 * dist * (fract - fract * fract);

  return false;
}

void StandardMove::startMove(ChessPiece* piece, ChessBoard& board, int param) {
  glm::vec2 targetPos = this->getTargetPosition(piece, param);
  auto other          = board[targetPos.x][targetPos.y];
  if(other) other->kill();
}

void StandardMove::endeMove(ChessPiece* piece, ChessBoard& board, int param) {
  glm::vec2 pos       = piece->getPositon();
  board[pos.x][pos.y] = nullptr;

  glm::vec2 targetPos = this->getTargetPosition(piece, param);
  piece->setPosition(targetPos);
  board[targetPos.x][targetPos.y] = piece;
}
