#include "game-state.h"

#include "../chess-game.h"

GameState::GameState(ChessGame* game)
: game(game) {
}

GameState::~GameState() {
}

void GameState::nextState(GameState* state) {
  this->game->setNextState(state);
}
