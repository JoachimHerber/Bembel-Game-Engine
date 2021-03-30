#ifndef BEMBEL_CHESSPIECE_H
#define BEMBEL_CHESSPIECE_H

#include <bembel/bembel.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "chess-game.h"
#include "moves/move-set.h"

class ChessPieceType;

class ChessPiece {
 public:
  ChessPiece(
    ChessPieceType*, bembel::kernel::Scene*, unsigned, const glm::ivec2&);

  void promote(ChessPieceType*);

  ChessPieceType* getType() const;
  bembel::kernel::Scene* getScene() const;
  unsigned getOwner() const;

  const glm::ivec2& getPositon() const;
  void setPosition(const glm::ivec2& pos);

  bembel::kernel::Scene::EntityID getEntity();

  bool isAlive() const;
  void kill();
  void reset();

  bool hasMoved() const;

  void updatePossibleMoves(const ChessBoard&);

  const std::vector<MoveSet::Move>& getPossibleMoves() const;

 private:
  bembel::kernel::Scene* scene;

  ChessPieceType* type;
  ChessPieceType* original_type;
  unsigned owner;

  glm::ivec2 positon;
  glm::ivec2 start_positon;

  bool is_alive;
  bool has_moved;

  bembel::kernel::Scene::EntityID entity;

  std::vector<MoveSet::Move> possible_moves;
};
#endif // include guards
