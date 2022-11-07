module;
#include <bembel/pch.h>
module bembel.examples.chess.board;

import bembel;
import bembel.examples.chess.selection;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;

ChessBoard::ChessBoard(Scene* scene) : m_scene{scene} {
    m_scene->registerComponentType<GeometryComponent>();
    m_scene->registerComponentType<PositionComponent>();
    m_scene->registerComponentType<ChessPieceComponent>();
    m_scene->registerComponentType<SelectionHighlightComponent>();
    m_scene->registerComponentType<RotationComponent>();

    AssetManager& asste_mgr = m_scene->getAssetManager();
    m_models[u8(PAWN)][u8(WHITE)].get(asste_mgr, "white.pawn");
    m_models[u8(PAWN)][u8(BLACK)].get(asste_mgr, "black.pawn");
    m_models[u8(ROOK)][u8(WHITE)].get(asste_mgr, "white.rook");
    m_models[u8(ROOK)][u8(BLACK)].get(asste_mgr, "black.rook");
    m_models[u8(KNIGHT)][u8(WHITE)].get(asste_mgr, "white.knight");
    m_models[u8(KNIGHT)][u8(BLACK)].get(asste_mgr, "black.knight");
    m_models[u8(BISHOP)][u8(WHITE)].get(asste_mgr, "white.bishop");
    m_models[u8(BISHOP)][u8(BLACK)].get(asste_mgr, "black.bishop");
    m_models[u8(QUEEN)][u8(WHITE)].get(asste_mgr, "white.queen");
    m_models[u8(QUEEN)][u8(BLACK)].get(asste_mgr, "black.queen");
    m_models[u8(KING)][u8(WHITE)].get(asste_mgr, "white.king");
    m_models[u8(KING)][u8(BLACK)].get(asste_mgr, "black.king");

    auto whiteTile = m_scene->getAssetHandle<GeometryModel>("white.tile");
    auto blackTile = m_scene->getAssetHandle<GeometryModel>("black.tile");

    for(unsigned u = 0; u < 8; ++u) {
        for(unsigned v = 0; v < 8; ++v) {
            m_tiles[u][v] = TilesEntity{*m_scene};

            auto& [pos, geom, highlight] = m_tiles[u][v];
            pos                          = vec3(2.0f * u, 0, 2.0f * v);
            geom.m_model                 = (u + v) % 2 != 0 ? whiteTile : blackTile;
            highlight.highlight          = SelectionHighlight::NO_HIGHLIGHT;
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
    for(unsigned u = 0; u < 8; ++u) {
        for(unsigned v = 0; v < 8; ++v) {
            if(m_board[u][v]) m_board[u][v].deleteEntity();
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

    ChessPieceEntity& chess_piece = m_board[pos.x][pos.y];

    if(chess_piece) chess_piece.deleteEntity();

    chess_piece                                        = ChessPieceEntity{*m_scene};
    auto& [piece, position, rotation, geom, selection] = chess_piece;

    piece.type          = type;
    piece.owner         = owner;
    piece.position      = pos;
    position            = vec3(2.0f * pos.x, 0, 2.0f * pos.y);
    rotation            = (owner == WHITE) ? quat{0, 0, 1, 0} : quat{1, 0, 0, 0};
    geom.m_model        = m_models[type][owner].getHandle();
    selection.highlight = SelectionHighlight::NO_HIGHLIGHT;
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
    from = ChessPieceEntity{};

    if(auto chess_piece = to.getComponent<ChessPieceComponent>()) {
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