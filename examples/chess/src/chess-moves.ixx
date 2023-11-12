module;
#include <vector>
export module bembel.examples.chess:Moves;

import bembel;
import :Board;

namespace bembel::examples::chess {
using namespace base;

struct Move {
    enum MoveType {
        MOVE,
        CAPUTE,
        CAPUTE_EN_PASSANT,
        CASTLING,
    };
    MoveType type;
    ivec2    to;
};

std::vector<Move> getPossibleMoves(ChessBoard* board , ChessPiece pice);

} // namespace bembel::examples::chess