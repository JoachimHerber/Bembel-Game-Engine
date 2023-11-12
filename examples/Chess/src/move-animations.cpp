module;
#include <chrono>
#include <cmath>
#include <coroutine>
#include <string_view>
module bembel.examples.chess;

import bembel;
import :Board;
import :Moves;
import :MoveAnimation;

namespace bembel::examples::chess {
using namespace bembel::base;
using namespace bembel::kernel;

MoveAnimation playMoveAnimation(ChessPiece chess_piece, ivec2 to, Signal<>& frame_sync) {
    ivec2 from = chess_piece.getBoardPosition();
    ivec2 posΔ = to - from;

    float dist = std::sqrt(float(posΔ.x * posΔ.x + posΔ.y * posΔ.y));

    vec3 start = {
        2.0f * from.x,
        0,
        2.0f * from.y,
    };
    vec3 dir = {
        2.0f * posΔ.x / dist,
        0,
        2.0f * posΔ.y / dist,
    };
    float hop_height = 0;
    float hop_dist   = 0;
    if(dir.x == 0 || dir.y == 0) {
        hop_height = 1;
        hop_dist   = 1;
    } else if(abs(dir.x) == abs(dir.y)) {
        hop_height = 1;
        hop_dist   = 1.414213562373095; // sqrt(2)
    } else {
        hop_height = dist;
        hop_dist   = dist;
    }
    float progress = 0;

    using namespace std::chrono;
    auto start_time = steady_clock::now();
    while(progress < dist) {
        vec3 pos = start + progress * dir;

        double fract = progress / hop_dist - std::floor(progress / hop_dist);
        pos.y        = 4 * hop_height * (fract - fract * fract);

        chess_piece.setPosition(pos);

        co_await frame_sync;
        progress = duration_cast<duration<float>>(steady_clock::now() - start_time).count();
    }
    chess_piece.setPosition(start + dist * dir);

    co_return;
}

} // namespace bembel::examples::chess
