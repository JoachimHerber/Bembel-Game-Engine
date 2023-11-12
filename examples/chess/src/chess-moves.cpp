module;
#include <cmath>
#include <coroutine>
#include <memory>
#include <string_view>
module bembel.examples.chess;

import bembel;
import :Board;
import :Moves;

namespace bembel::examples::chess {
using namespace base;

bool addMove(
    ChessBoard* board, ChessPiece pice, std::vector<Move>& moves, ivec2 pos, bool can_capture
) {
    if(pos.x < 0 || 8 <= pos.x) return false;
    if(pos.y < 0 || 8 <= pos.y) return false;

    auto other = board->getChessPieceAt(pos);
    if(!other) {
        moves.emplace_back(Move::MOVE, pos);
        return true;
    }
    if(can_capture && other.getOwner() != pice.getOwner()) moves.emplace_back(Move::CAPUTE, pos);
    return false;
}

void getPossiblePawnMoves(ChessBoard* board, ChessPiece pice, std::vector<Move>& moves) {
    ivec2 pos = pice.getBoardPosition();
    int   dir = (pice.getOwner() == ChessPlayer::WHITE) ? 1 : -1;

    if(addMove(board, pice, moves, pos + ivec2(dir, 0), false)) {
        if(!pice.hasMoved()) { addMove(board, pice, moves, pos + ivec2(2 * dir, 0), false); }
    }
    if(auto other = board->getChessPieceAt(pos + ivec2(dir, -1))) {
        if(other.getOwner() != pice.getOwner())
            moves.emplace_back(Move::CAPUTE, pos + ivec2(dir, -1));
    }
    if(board->canCaptureEnPassant(pos + ivec2(dir, -1))) {
        moves.emplace_back(Move::CAPUTE_EN_PASSANT, pos + ivec2(dir, -1));
    }
    if(auto other = board->getChessPieceAt(pos + ivec2(dir, +1))) {
        if(other.getOwner() != pice.getOwner())
            moves.emplace_back(Move::CAPUTE, pos + ivec2(dir, +1));
    }
    if(board->canCaptureEnPassant(pos + ivec2(dir, +1))) {
        moves.emplace_back(Move::CAPUTE_EN_PASSANT, pos + ivec2(dir, +1));
    }
}

void getPossibleKnightMoves(ChessBoard* board, ChessPiece pice, std::vector<Move>& moves) {
    ivec2 pos = pice.getBoardPosition();

    addMove(board, pice, moves, pos + ivec2(+1, +2), true);
    addMove(board, pice, moves, pos + ivec2(+2, +1), true);
    addMove(board, pice, moves, pos + ivec2(+2, -1), true);
    addMove(board, pice, moves, pos + ivec2(+1, -2), true);
    addMove(board, pice, moves, pos + ivec2(-1, -2), true);
    addMove(board, pice, moves, pos + ivec2(-2, -1), true);
    addMove(board, pice, moves, pos + ivec2(-2, +1), true);
    addMove(board, pice, moves, pos + ivec2(-1, +2), true);
}

void getPossibleRookMoves(ChessBoard* board, ChessPiece pice, std::vector<Move>& moves) {
    ivec2 pos = pice.getBoardPosition();

    for(int i = 1; addMove(board, pice, moves, pos + ivec2(+i, 0), true); ++i) {}
    for(int i = 1; addMove(board, pice, moves, pos + ivec2(-i, 0), true); ++i) {}
    for(int i = 1; addMove(board, pice, moves, pos + ivec2(0, +i), true); ++i) {}
    for(int i = 1; addMove(board, pice, moves, pos + ivec2(0, -i), true); ++i) {}
}

void getPossibleBishopMoves(ChessBoard* board, ChessPiece pice, std::vector<Move>& moves) {
    ivec2 pos = pice.getBoardPosition();

    for(int i = 1; addMove(board, pice, moves, pos + ivec2(-i, -i), true); ++i) {}
    for(int i = 1; addMove(board, pice, moves, pos + ivec2(-i, +i), true); ++i) {}
    for(int i = 1; addMove(board, pice, moves, pos + ivec2(+i, -i), true); ++i) {}
    for(int i = 1; addMove(board, pice, moves, pos + ivec2(+i, +i), true); ++i) {}
}

void getPossibleKingMoves(ChessBoard* board, ChessPiece pice, std::vector<Move>& moves) {
    ivec2 pos = pice.getBoardPosition();

    addMove(board, pice, moves, pos + ivec2(+1, +0), true);
    addMove(board, pice, moves, pos + ivec2(+1, +1), true);
    addMove(board, pice, moves, pos + ivec2(+0, +1), true);
    addMove(board, pice, moves, pos + ivec2(-1, +1), true);
    addMove(board, pice, moves, pos + ivec2(-1, +0), true);
    addMove(board, pice, moves, pos + ivec2(-1, -1), true);
    addMove(board, pice, moves, pos + ivec2(+0, -1), true);
    addMove(board, pice, moves, pos + ivec2(+1, -1), true);
}

std::vector<Move> getPossibleMoves(ChessBoard* board, ChessPiece pice) {
    std::vector<Move> moves;

    if(!pice) return moves;

    switch(pice.getType()) {
        case ChessPieceType::PAWN: getPossiblePawnMoves(board, pice, moves); break;
        case ChessPieceType::ROOK: getPossibleRookMoves(board, pice, moves); break;
        case ChessPieceType::KNIGHT: getPossibleKnightMoves(board, pice, moves); break;
        case ChessPieceType::BISHOP: getPossibleBishopMoves(board, pice, moves); break;
        case ChessPieceType::QUEEN: {
            getPossibleRookMoves(board, pice, moves);
            getPossibleBishopMoves(board, pice, moves);
            break;
        }
        case ChessPieceType::KING: getPossibleKingMoves(board, pice, moves); break;
    }
    return moves;
}

} // namespace bembel::examples::chess
