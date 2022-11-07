module;
#include "bembel/pch.h"
export module bembel.examples.chess.move_animations;

import bembel;
import bembel.examples.chess.board;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;

export class MoveAnimation {
  public:
    virtual ~MoveAnimation() = default;

    virtual bool update(double time) = 0;
};

export class StandartMoveAnimation : public MoveAnimation {
  public:
    StandartMoveAnimation(ChessPiece, ivec2 to);

    bool update(double time) override;

  private:
    ChessPiece m_chess_piece;

    vec3  m_start;
    vec3  m_dir;
    float m_dist;
    float m_hop_dist;
    float m_hop_height;

    float m_progress = 0;
};

} // namespace bembel::examples::chess