module;
#include <array>
#include <string_view>
export module bembel.examples.chess:Board;

import bembel;
import :SelectionHighlight;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace physics::units::literals;

export enum ChessPieceType : u8 { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
export enum ChessPlayer : u8 { WHITE, BLACK };

export struct ChessPieceComponent {
    using Container = ComponentVector<ChessPieceComponent>;

    ChessPieceType type;
    ChessPlayer    owner;
    ivec2          position;
    bool           has_moved = false;
};

export bool initComponent(xml::Element const*, ChessPieceComponent&) {
    return true;
}
class ChessBoard;

export class ChessPiece {
  public:
    ChessPiece() = default;
    ChessPiece(ChessBoard* board, Entity entity) : m_board(board), m_entity(entity) {}
    ChessPiece(ChessPiece const&)            = default;
    ChessPiece(ChessPiece&&)                 = default;
    ChessPiece& operator=(ChessPiece const&) = default;
    ChessPiece& operator=(ChessPiece&&)      = default;

    bool operator==(ChessPiece const& other) const {
        return m_board == other.m_board && m_entity == other.m_entity;
    }

    operator bool() { return m_board && m_entity; }

    ChessPieceType getType()  { return m_entity.get<ChessPieceComponent>()->type; }
    ChessPlayer    getOwner() { return m_entity.get<ChessPieceComponent>()->owner; }
    bool           hasMoved() { return m_entity.get<ChessPieceComponent>()->has_moved; }

    void  setBoardPosition(ivec2 pos);
    ivec2 getBoardPosition() { return m_entity.get<ChessPieceComponent>()->position; }

    vec3& getPosition() { return m_entity.get<Transform>()->position; }
    void  setPosition(vec3 pos) { m_entity.get<Transform>()->position = pos; }

    quat& getRotation() { return m_entity.get<Transform>()->rotation; }
    void  setRotation(quat rot) { m_entity.get<Transform>()->rotation = rot; }

    SelectionHighlight& getHighlight() { return *m_entity.get<SelectionHighlight>(); }
    void                setHighlight(SelectionHighlight highlight) {
        m_entity.assign<SelectionHighlight>(highlight);
    }

    void makeRigidBodyKinematic() { m_entity.get<RigidBody>()->makeKinematic(); }
    void makeRigidBodyStatic() { m_entity.get<RigidBody>()->makeStatic(); }
    void makeRigidBodyDynamic() { m_entity.get<RigidBody>()->makeDynamic(1_kg); }
    vec3 getRigidBodyLinearVelocity() { return m_entity.get<RigidBody>()->getLinearVelocity(); }

  private:
    ChessBoard* m_board = nullptr;
    Entity      m_entity;
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
    Entity getTileAt(ivec2 pos) const {
        if(pos.x < 0 || 8 <= pos.x) return {};
        if(pos.y < 0 || 8 <= pos.y) return {};
        return m_tiles[pos.x][pos.y];
    }

    bool canCaptureEnPassant(ivec2 pos);

    void captureChessPieceAt(ivec2 pos);

    void movePiece(ivec2 from, ivec2 to);
    void promote(ivec2 pos, ChessPieceType new_type);

    Scene* getScene() { return m_scene; }

  private:
    Scene*                               m_scene;
    std::array<std::array<Entity, 8>, 8> m_board;
    std::array<std::array<Entity, 8>, 8> m_tiles;

    struct Assets {
        Asset<GeometryModel>  models[2];
        Asset<CollisionShape> collision_shape;

        void request(
            std::string_view model_white_name,
            std::string_view model_black_name,
            std::string_view collision_shape_name
        ) {
            models[u8(WHITE)].request(model_white_name);
            models[u8(BLACK)].request(model_black_name);
            collision_shape.request(collision_shape_name);
        }
    };

    std::array<Assets, 7> m_assets;

    std::optional<ivec2> m_en_passant;
};

inline void ChessPiece::setBoardPosition(ivec2 pos) {
    m_board->movePiece(getBoardPosition(), pos);
}

} // namespace bembel::examples::chess