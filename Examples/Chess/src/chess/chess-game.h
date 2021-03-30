#ifndef BEMBEL_CHESSGAME_H
#define BEMBEL_CHESSGAME_H

#include <array>
#include <bembel/bembel.hpp>
#include <memory>
#include <vector>

#include "player.h"

#pragma region ForwardDeclarations
class SelectionComponent;
class GameState;
class ChessPieceType;
class SelectionPointer;
#pragma endregion ForwardDeclarations

using ChessBoard = std::array<std::array<ChessPiece*, 8>, 8>;

class ChessGame {
  public:
    ChessGame(
        bembel::kernel::AssetManager&,
        bembel::kernel::EventManager&,
        bembel::graphics::GraphicSystem&);
    ~ChessGame();

    std::shared_ptr<bembel::kernel::Scene> getScene() const;

    ChessBoard&       getChessBoard();
    const ChessBoard& getChessBoard() const;

    const std::array<Player, 2>& getPlayers() const;

    enum { PAWN = 0, ROOK, KNIGHT, BISHOP, QUEEN, KING };

    void resetChessBoard();
    void updatePossibleMoves();

    SelectionComponent* getBoardTileSelectionComponent(const glm::ivec2&);

    void update(double time);
    void setNextState(GameState* state);

  private:
    void initTiles();
    void initChessPieceTypes();
    void initChessPices();
    void initStates();

    std::unique_ptr<ChessPieceType> createChessPieceType(const std::string&);
    void addChessPiece(const glm::vec2& pos, unsigned type, unsigned owner);

  private:
    std::shared_ptr<bembel::kernel::Scene> scene;

    std::unique_ptr<SelectionPointer> selection_pointer;

    std::array<Player, 2> players;

    std::vector<std::unique_ptr<ChessPieceType>> chess_piece_types;
    std::vector<std::unique_ptr<ChessPiece>>     chess_pieces;

    ChessBoard board;

    std::array<std::array<bembel::kernel::Scene::EntityID, 8>, 8> tiles;

    std::vector<std::unique_ptr<GameState>> states;

    GameState* current_state = nullptr;
    GameState* next_state    = nullptr;
};
#endif // include guards
