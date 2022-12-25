module;
#include <cmath>
#include <string_view>
module bembel.examples.chess.move_animations;

namespace bembel::examples::chess {
using namespace bembel::base;
using namespace bembel::kernel;

StandartMoveAnimation::StandartMoveAnimation(ChessPiece chess_piece, ivec2 to)
  : m_chess_piece(chess_piece) {
    ivec2 from = m_chess_piece.getBoardPosition();
    ivec2 dir  = to - from;

    m_dist = std::sqrt(float(dir.x * dir.x + dir.y * dir.y));

    m_start = vec3{
        2.0f * from.x,
        0,
        2.0f * from.y,
    };
    m_dir = vec3{
        2.0f * dir.x / m_dist,
        0,
        2.0f * dir.y / m_dist,
    };
    if(dir.x == 0 || dir.y == 0) {
        m_hop_height = 1;
        m_hop_dist   = 1;
    } else if(abs(dir.x) == abs(dir.y)) {
        m_hop_height = 1;
        m_hop_dist   = 1.414213562373095; // sqrt(2)
    } else {
        m_hop_height = m_dist;
        m_hop_dist   = m_dist;
    }
}

bool StandartMoveAnimation::update(double time) {
    m_progress += time;

    if(m_progress > m_dist) m_progress = m_dist;

    vec3 pos = m_start + m_progress * m_dir;

    double fract = m_progress / m_hop_dist - std::floor(m_progress / m_hop_dist);
    pos.y        = 4 * m_hop_height * (fract - fract * fract);

    m_chess_piece.setPosition(pos);
    return m_progress >= m_dist;
}

} // namespace bembel::examples::chess
