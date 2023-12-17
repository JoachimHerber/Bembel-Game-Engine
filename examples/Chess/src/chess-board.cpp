module;
#include <cstdlib>
#include <source_location>
#include <string_view>
module bembel.examples.chess:Board;

import bembel;
import :SelectionHighlight;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace physics::units::literals;

constexpr std::array<vec3, 6> CHESS_PIECE_CENTER_OF_MASS = {
    {{0.0f, 1.0f, 0.0f},
     {0.0f, 1.1f, 0.0f},
     {0.0f, 1.1f, 0.0f},
     {0.0f, 1.5f, 0.0f},
     {0.0f, 1.6f, 0.0f},
     {0.0f, 1.6f, 0.0f}}};

ChessBoard::ChessBoard(Scene* scene) : m_scene{scene} {
    m_scene->registerComponentType<Geometry>();
    m_scene->registerComponentType<Transform>();
    m_scene->registerComponentType<ChessPieceComponent>();
    m_scene->registerComponentType<SelectionHighlight>();

    // clang-format off
    m_assets[u8(PAWN)]  .request("white.pawn",   "black.pawn",   "pawn");
    m_assets[u8(ROOK)]  .request("white.rook",   "black.rook",   "rook");
    m_assets[u8(KNIGHT)].request("white.knight", "black.knight", "knight");
    m_assets[u8(BISHOP)].request("white.bishop", "black.bishop", "bishop");
    m_assets[u8(QUEEN)] .request("white.queen",  "black.queen",  "queen");
    m_assets[u8(KING)]  .request("white.king",   "black.king",   "king");
    m_assets.back()     .request("white.tile",   "black.tile",   "tile");
    // clang-format on

    for(unsigned u = 0; u < 8; ++u) {
        for(unsigned v = 0; v < 8; ++v) {
            Entity tile{*m_scene};

            tile.assign<Transform>(vec3(2.0f * u, 0, 2.0f * v));
            tile.assign<Geometry>(m_assets.back().models[(u + v) % 2]);
            tile.assign<SelectionHighlight>(SelectionHighlight::NO_HIGHLIGHT);
            tile.assign<RigidBody>(m_assets.back().collision_shape, vec3{0, 0, 0}, 0_kg);

            m_tiles[u][v] = tile;
        }
    }
    resetBoard();
}

ChessBoard::~ChessBoard() {
    for(unsigned u = 0; u < 8; ++u) {
        for(unsigned v = 0; v < 8; ++v) {
            m_tiles[u][v].deleteEntity();
            m_board[u][v].deleteEntity();
        }
    }
}

void ChessBoard::resetBoard() {
    for(auto& row : m_board) {
        for(auto& chess_piece : row) {
            if(chess_piece) chess_piece.deleteEntity();
        }
    }
    createChessPiece({0, 0}, ROOK, WHITE);
    createChessPiece({0, 1}, KNIGHT, WHITE);
    createChessPiece({0, 2}, BISHOP, WHITE);
    createChessPiece({0, 3}, QUEEN, WHITE);
    createChessPiece({0, 4}, KING, WHITE);
    createChessPiece({0, 5}, BISHOP, WHITE);
    createChessPiece({0, 6}, KNIGHT, WHITE);
    createChessPiece({0, 7}, ROOK, WHITE);

    createChessPiece({1, 0}, PAWN, WHITE);
    createChessPiece({1, 1}, PAWN, WHITE);
    createChessPiece({1, 2}, PAWN, WHITE);
    createChessPiece({1, 3}, PAWN, WHITE);
    createChessPiece({1, 4}, PAWN, WHITE);
    createChessPiece({1, 5}, PAWN, WHITE);
    createChessPiece({1, 6}, PAWN, WHITE);
    createChessPiece({1, 7}, PAWN, WHITE);

    createChessPiece({6, 0}, PAWN, BLACK);
    createChessPiece({6, 1}, PAWN, BLACK);
    createChessPiece({6, 2}, PAWN, BLACK);
    createChessPiece({6, 3}, PAWN, BLACK);
    createChessPiece({6, 4}, PAWN, BLACK);
    createChessPiece({6, 5}, PAWN, BLACK);
    createChessPiece({6, 6}, PAWN, BLACK);
    createChessPiece({6, 7}, PAWN, BLACK);

    createChessPiece({7, 0}, ROOK, BLACK);
    createChessPiece({7, 1}, KNIGHT, BLACK);
    createChessPiece({7, 2}, BISHOP, BLACK);
    createChessPiece({7, 3}, QUEEN, BLACK);
    createChessPiece({7, 4}, KING, BLACK);
    createChessPiece({7, 5}, BISHOP, BLACK);
    createChessPiece({7, 6}, KNIGHT, BLACK);
    createChessPiece({7, 7}, ROOK, BLACK);
}

void ChessBoard::createChessPiece(ivec2 pos, ChessPieceType type, ChessPlayer owner) {
    if(pos.x >= 8 || pos.y >= 8) return;

    Entity& chess_piece = m_board[pos.x][pos.y];

    if(chess_piece) chess_piece.deleteEntity();

    chess_piece = Entity{*m_scene};

    chess_piece.assign<Transform>(
        vec3(2.0f * pos.x, 0, 2.0f * pos.y),
        1.0f,
        (owner == WHITE) ? quat{0, 0, 1, 0} : quat{1, 0, 0, 0}
    );
    chess_piece.assign<ChessPieceComponent>(type, owner, pos);
    chess_piece.assign<Geometry>(m_assets[type].models[owner]);
    chess_piece.assign<SelectionHighlight>(SelectionHighlight::NO_HIGHLIGHT);
    chess_piece.assign<RigidBody>(
        m_assets[type].collision_shape, CHESS_PIECE_CENTER_OF_MASS[type], 0_kg
    );
}

bool ChessBoard::canCaptureEnPassant(ivec2 pos) {
    return m_en_passant.has_value() && m_en_passant.value() == pos;
}

void ChessBoard::captureChessPieceAt(ivec2 pos) {
    if(pos.x < 0 || 8 <= pos.x) return;
    if(pos.y < 0 || 8 <= pos.y) return;

    m_board[pos.x][pos.y].deleteEntity();
}

void ChessBoard::movePiece(ivec2 from_pos, ivec2 to_pos) {
    auto& from = m_board[from_pos.x][from_pos.y];
    auto& to   = m_board[to_pos.x][to_pos.y];

    if(to) to.deleteEntity();

    to   = from;
    from = Entity{};

    if(to) {
        auto* chess_piece = to.get<ChessPieceComponent>();
        if(chess_piece->type == ChessPieceType::PAWN) {
            if(canCaptureEnPassant(to_pos)) { m_board[from_pos.x][to_pos.y].deleteEntity(); }

            if(!chess_piece->has_moved && abs(from_pos.x - to_pos.x) == 2) {
                m_en_passant = ivec2((from_pos.x + to_pos.x) / 2, to_pos.y);
            } else {
                m_en_passant.reset();
            }
        } else {
            m_en_passant.reset();
        }

        chess_piece->position  = to_pos;
        chess_piece->has_moved = true;
    }
}
void ChessBoard::promote(ivec2 pos, ChessPieceType new_type) {}

} // namespace bembel::examples::chess
