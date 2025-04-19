export module bembel.examples.chess:MoveAnimation;

import std;
import bembel;
import :Board;
import :Moves;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;

export coro::Task<> playMoveAnimation(ChessPiece chess_piece, ivec2 to);

} // namespace bembel::examples::chess