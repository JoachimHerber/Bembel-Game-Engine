
#ifndef BEMBEL_CHESSPIECETYPE_H
#define BEMBEL_CHESSPIECETYPE_H

#include <array>
#include <bembel/bembel.hpp>
#include <map>
#include <memory>

#include "moves/move-set.h"

class ChessPieceType {
 public:
  ChessPieceType();
  ~ChessPieceType();

  std::array<bembel::kernel::AssetHandle, 2>& getModles();
  const std::array<bembel::kernel::AssetHandle, 2>& getModles() const;

  void addMove(
    const glm::ivec2& dir,
    unsigned max_dist,
    bool attack = true,
    bool move   = true);
  void addMove(const glm::ivec2& dir, bool attack = true, bool move = true);
  MoveSet& getMoveSet();

 private:
  std::array<bembel::kernel::AssetHandle, 2> modles;

  MoveSet move_set;
};
#endif // include guards
