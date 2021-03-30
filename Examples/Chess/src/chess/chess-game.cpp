#include "chess-game.h"

#include "../selection-component.h"
#include "chess-piece-type.h"
#include "chess-piece.h"
#include "game-logic/perform-move.h"
#include "game-logic/select-chess-piece.h"
#include "game-logic/select-move.h"
#include "game-logic/selection-pointer.h"
#include "moves/initial-pawn-move.h"

using namespace bembel::kernel;
using namespace bembel::graphics;

ChessGame::ChessGame(AssetManager& assetMgr, EventManager& eventMgr, GraphicSystem& graphicSys)
: scene{std::make_shared<Scene>(assetMgr)}
, selection_pointer(std::make_unique<SelectionPointer>(eventMgr, graphicSys, scene.get()))
, players{Player{this, "white"}, Player{this, "black"}} {
    this->scene->registerComponentType<PositionComponent>();
    this->scene->registerComponentType<RotationComponent>();
    this->scene->registerComponentType<DirectionalLightSource>();
    this->scene->registerComponentType<GeometryComponent>();
    this->scene->registerComponentType<SelectionComponent>();

    this->scene->loadAssets("assets/assets.xml");

    auto entity      = this->scene->createEntity();
    auto light       = this->scene->createComponent<DirectionalLightSource>(entity);
    light->direction = glm::normalize(glm::vec3(-0.3, -1, -0.2));
    light->color     = 5.0f * glm::vec3(1, 1, 1);

    this->initTiles();
    this->initChessPieceTypes();
    this->initChessPices();
    this->initStates();
}

ChessGame::~ChessGame() {
}

std::shared_ptr<Scene> ChessGame::getScene() const {
    return this->scene;
}

ChessBoard& ChessGame::getChessBoard() {
    return this->board;
}

const ChessBoard& ChessGame::getChessBoard() const {
    return this->board;
}

const std::array<Player, 2>& ChessGame::getPlayers() const {
    return this->players;
}

void ChessGame::resetChessBoard() {
    this->players[0].clearChessPieces();
    this->players[1].clearChessPieces();
    this->players[0].clearCaptureChessPieces();
    this->players[1].clearCaptureChessPieces();

    for(auto& it1 : this->board) {
        for(auto& it2 : it1) { it2 = nullptr; }
    }

    for(auto& it : this->chess_pieces) {
        it->reset();
        this->players[it->getOwner()].addChessPiece(it.get());

        auto pos                  = it->getPositon();
        this->board[pos.x][pos.y] = it.get();
    }
}

void ChessGame::updatePossibleMoves() {
    for(auto& it : this->chess_pieces) it->updatePossibleMoves(this->board);
}

SelectionComponent* ChessGame::getBoardTileSelectionComponent(const glm::ivec2& pos) {
    if(pos.x < 0 || pos.x >= 8 || pos.y < 0 || pos.y >= 8) return nullptr;

    return this->scene->getComponent<SelectionComponent>(this->tiles[pos.x][pos.y]);
}

void ChessGame::update(double time) {
    if(this->next_state != nullptr) {
        if(this->current_state != nullptr) this->current_state->onExitState();

        this->current_state = this->next_state;
        this->next_state    = nullptr;

        this->current_state->onEnterState();
    }

    if(this->current_state != nullptr) { this->current_state->update(time); }
}

void ChessGame::setNextState(GameState* state) {
    this->next_state = state;
}

void ChessGame::initTiles() {
    auto whiteTile = this->scene->getAssetHandle<GeometryModel>("white.tile");
    auto blackTile = this->scene->getAssetHandle<GeometryModel>("black.tile");

    for(unsigned u = 0; u < 8; ++u) {
        for(unsigned v = 0; v < 8; ++v) {
            auto tile         = this->scene->createEntity();
            this->tiles[u][v] = tile;

            auto geomety       = this->scene->createComponent<GeometryComponent>(tile);
            auto position      = this->scene->createComponent<PositionComponent>(tile);
            auto selectComp    = this->scene->createComponent<SelectionComponent>(tile);
            geomety->model     = ((u + v) % 2 != 0 ? whiteTile : blackTile);
            position->position = glm::vec3(2.0f * u, 0, 2.0f * v);
            selectComp->state  = SelectionComponent::State::UNSELECTABLE;
        }
    }
}

void ChessGame::initChessPieceTypes() {
    this->chess_piece_types.resize(6);
    this->chess_piece_types[PAWN]   = this->createChessPieceType("pawn");
    this->chess_piece_types[ROOK]   = this->createChessPieceType("rook");
    this->chess_piece_types[KNIGHT] = this->createChessPieceType("knight");
    this->chess_piece_types[BISHOP] = this->createChessPieceType("bishop");
    this->chess_piece_types[QUEEN]  = this->createChessPieceType("queen");
    this->chess_piece_types[KING]   = this->createChessPieceType("king");

    this->chess_piece_types[PAWN]->addMove({1, -1}, 1, true, false);
    this->chess_piece_types[PAWN]->addMove({1, +0}, 1, false, true);
    this->chess_piece_types[PAWN]->addMove({1, +1}, 1, true, false);
    this->chess_piece_types[PAWN]->getMoveSet().addMoveTemplate(
        std::make_shared<InitialPawnMove>());

    this->chess_piece_types[ROOK]->addMove({1, 0});
    this->chess_piece_types[ROOK]->addMove({0, -1});
    this->chess_piece_types[ROOK]->addMove({-1, 0});
    this->chess_piece_types[ROOK]->addMove({0, +1});

    this->chess_piece_types[KNIGHT]->addMove({+1, +2}, 1U);
    this->chess_piece_types[KNIGHT]->addMove({+2, +1}, 1U);
    this->chess_piece_types[KNIGHT]->addMove({+2, -1}, 1U);
    this->chess_piece_types[KNIGHT]->addMove({+1, -2}, 1U);
    this->chess_piece_types[KNIGHT]->addMove({-1, -2}, 1U);
    this->chess_piece_types[KNIGHT]->addMove({-2, -1}, 1U);
    this->chess_piece_types[KNIGHT]->addMove({-2, +1}, 1U);
    this->chess_piece_types[KNIGHT]->addMove({-1, +2}, 1U);

    this->chess_piece_types[BISHOP]->addMove({+1, +1});
    this->chess_piece_types[BISHOP]->addMove({+1, -1});
    this->chess_piece_types[BISHOP]->addMove({-1, -1});
    this->chess_piece_types[BISHOP]->addMove({-1, +1});

    this->chess_piece_types[QUEEN]->addMove({+1, +1});
    this->chess_piece_types[QUEEN]->addMove({+1, 0});
    this->chess_piece_types[QUEEN]->addMove({+1, -1});
    this->chess_piece_types[QUEEN]->addMove({0, -1});
    this->chess_piece_types[QUEEN]->addMove({-1, -1});
    this->chess_piece_types[QUEEN]->addMove({-1, 0});
    this->chess_piece_types[QUEEN]->addMove({-1, +1});
    this->chess_piece_types[QUEEN]->addMove({0, +1});

    this->chess_piece_types[KING]->addMove({+1, +1}, 1U);
    this->chess_piece_types[KING]->addMove({+1, 0}, 1U);
    this->chess_piece_types[KING]->addMove({+1, -1}, 1U);
    this->chess_piece_types[KING]->addMove({0, -1}, 1U);
    this->chess_piece_types[KING]->addMove({-1, -1}, 1U);
    this->chess_piece_types[KING]->addMove({-1, 0}, 1U);
    this->chess_piece_types[KING]->addMove({-1, +1}, 1U);
    this->chess_piece_types[KING]->addMove({0, +1}, 1U);
}

void ChessGame::initChessPices() {
    this->addChessPiece({0, 0}, ROOK, 0);
    this->addChessPiece({0, 1}, KNIGHT, 0);
    this->addChessPiece({0, 2}, BISHOP, 0);
    this->addChessPiece({0, 3}, QUEEN, 0);
    this->addChessPiece({0, 4}, KING, 0);
    this->addChessPiece({0, 5}, BISHOP, 0);
    this->addChessPiece({0, 6}, KNIGHT, 0);
    this->addChessPiece({0, 7}, ROOK, 0);

    this->addChessPiece({1, 0}, PAWN, 0);
    this->addChessPiece({1, 1}, PAWN, 0);
    this->addChessPiece({1, 2}, PAWN, 0);
    this->addChessPiece({1, 3}, PAWN, 0);
    this->addChessPiece({1, 4}, PAWN, 0);
    this->addChessPiece({1, 5}, PAWN, 0);
    this->addChessPiece({1, 6}, PAWN, 0);
    this->addChessPiece({1, 7}, PAWN, 0);

    this->addChessPiece({6, 0}, PAWN, 1);
    this->addChessPiece({6, 1}, PAWN, 1);
    this->addChessPiece({6, 2}, PAWN, 1);
    this->addChessPiece({6, 3}, PAWN, 1);
    this->addChessPiece({6, 4}, PAWN, 1);
    this->addChessPiece({6, 5}, PAWN, 1);
    this->addChessPiece({6, 6}, PAWN, 1);
    this->addChessPiece({6, 7}, PAWN, 1);

    this->addChessPiece({7, 0}, ROOK, 1);
    this->addChessPiece({7, 1}, KNIGHT, 1);
    this->addChessPiece({7, 2}, BISHOP, 1);
    this->addChessPiece({7, 3}, QUEEN, 1);
    this->addChessPiece({7, 4}, KING, 1);
    this->addChessPiece({7, 5}, BISHOP, 1);
    this->addChessPiece({7, 6}, KNIGHT, 1);
    this->addChessPiece({7, 7}, ROOK, 1);
}

void ChessGame::initStates() {
    auto select_chess_piece_0 =
        std::make_unique<SelectChessPieceState>(this, 0, this->selection_pointer.get());
    auto select_chess_piece_1 =
        std::make_unique<SelectChessPieceState>(this, 1, this->selection_pointer.get());
    auto select_move_0  = std::make_unique<SelectMoveState>(this, this->selection_pointer.get());
    auto select_move_1  = std::make_unique<SelectMoveState>(this, this->selection_pointer.get());
    auto perform_move_0 = std::make_unique<PerformMoveState>(this);
    auto perform_move_1 = std::make_unique<PerformMoveState>(this);

    select_chess_piece_0->setSelectMoveState(select_move_0.get());
    select_chess_piece_1->setSelectMoveState(select_move_1.get());
    select_move_0->setPerformMoveState(perform_move_0.get());
    select_move_1->setPerformMoveState(perform_move_1.get());
    perform_move_0->init(select_chess_piece_1.get());
    perform_move_1->init(select_chess_piece_0.get());

    this->states.push_back(std::move(select_chess_piece_0));
    this->states.push_back(std::move(select_chess_piece_1));
    this->states.push_back(std::move(select_move_0));
    this->states.push_back(std::move(select_move_1));
    this->states.push_back(std::move(perform_move_0));
    this->states.push_back(std::move(perform_move_1));

    this->next_state = this->states[0].get();
}

void ChessGame::addChessPiece(const glm::vec2& pos, unsigned type, unsigned owner) {
    this->chess_pieces.push_back(std::make_unique<ChessPiece>(
        this->chess_piece_types[type].get(), this->scene.get(), owner, pos));
}

std::unique_ptr<ChessPieceType> ChessGame::createChessPieceType(const std::string& modleSufix) {
    auto type = std::make_unique<ChessPieceType>();

    type->getModles()[0] =
        this->scene->getAssetHandle<GeometryModel>(this->players[0].getName() + "." + modleSufix);
    type->getModles()[1] =
        this->scene->getAssetHandle<GeometryModel>(this->players[1].getName() + "." + modleSufix);

    return std::move(type);
}
