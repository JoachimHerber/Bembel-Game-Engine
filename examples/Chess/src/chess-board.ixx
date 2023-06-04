module;
#include <array>
#include <string_view>
export module bembel.examples.chess.board;

import bembel;
import bembel.examples.chess.selection;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;

export enum ChessPieceType : u8 { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
export enum ChessPlayer : u8 { WHITE, BLACK };

export struct ChessPieceComponentData {
    ChessPieceType type;
    ChessPlayer    owner;
    ivec2          position;
    bool           has_moved = false;
};
export bool initComponent(xml::Element const*, ChessPieceComponentData&) {
    return true;
}
export using ChessPieceComponent =
    BasicComponent<"ChessPieceComponent", ChessPieceComponentData>;

class ChessBoard;

export using ChessPieceEntity = Entity<
    ChessPieceComponent,
    Transform,
    Geometry,
    SelectionHighlightComponent>;

export using TilesEntity =
    Entity<Transform, Geometry, SelectionHighlightComponent>;

export class ChessPiece {
  public:
    ChessPiece() = default;
    ChessPiece(ChessBoard* board, ChessPieceEntity entity) : m_board(board), m_entity(entity) {}
    ChessPiece(ChessPiece const&)            = default;
    ChessPiece(ChessPiece&&)                 = default;
    ChessPiece& operator=(ChessPiece const&) = default;
    ChessPiece& operator=(ChessPiece&&)      = default;

    bool operator==(ChessPiece const& other) const {
        return m_board == other.m_board && m_entity == other.m_entity;
    }

    operator bool() { return m_board && m_entity; }

    ChessPieceType getType() { return m_entity.getComponent<ChessPieceComponent>()->type; }
    ChessPlayer    getOwner() { return m_entity.getComponent<ChessPieceComponent>()->owner; }
    bool           hasMoved() { return m_entity.getComponent<ChessPieceComponent>()->has_moved; }

    void  setBoardPosition(ivec2 pos);
    ivec2 getBoardPosition() { return m_entity.getComponent<ChessPieceComponent>()->position; }

    vec3& getPosition() { return m_entity.getComponent<Transform>()->position; }
    void  setPosition(vec3 pos) { m_entity.getComponent<Transform>()->position = pos; }

    SelectionHighlight getHighlight() {
        return *(m_entity.getComponent<SelectionHighlightComponent>());
    }
    void setHighlight(SelectionHighlight highlight) {
        *(m_entity.getComponent<SelectionHighlightComponent>()) = highlight;
    }

  private:
    ChessBoard*      m_board = nullptr;
    ChessPieceEntity m_entity;
};

export class ChessBoard {
  public:
    ChessBoard(Scene*);
    ~ChessBoard();

    void resetBoard();

    void createChessPiece(ivec2 position, ChessPieceType, ChessPlayer);

    ChessPiece getChessPieceAt(ivec2 pos) {
        if(pos.x < 0 || 8 <= pos.x) return {};
        if(pos.y < 0 || 8 <= pos.y) return {};

        return {this, m_board[pos.x][pos.y]};
    }
    TilesEntity getTileAt(ivec2 pos) const {
        if(pos.x < 0 || 8 <= pos.x) return {};
        if(pos.y < 0 || 8 <= pos.y) return {};
        return m_tiles[pos.x][pos.y];
    }

    bool canCaptureEnPassant(ivec2 pos);

    void captureChessPieceAt(ivec2 pos);

    void movePiece(ivec2 from, ivec2 to);
    void promote(ivec2 pos, ChessPieceType new_type);

  private:
    Scene*                                         m_scene;
    std::array<std::array<ChessPieceEntity, 8>, 8> m_board;
    std::array<std::array<TilesEntity, 8>, 8>      m_tiles;

    std::array<std::array<Asset<GeometryModel>, 2>, 6> m_models;

    std::optional<ivec2> m_en_passant;
};

inline void ChessPiece::setBoardPosition(ivec2 pos) {
    m_board->movePiece(getBoardPosition(), pos);
}

} // namespace bembel::examples::chess