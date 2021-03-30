
#ifndef BEMBEL_GAMESTATE_H
#define BEMBEL_GAMESTATE_H

class ChessGame;

class GameState {
 public:
  GameState(ChessGame* game);
  virtual ~GameState();

  virtual void onEnterState(){};
  virtual void onExitState(){};

  virtual void update(double time){};

 protected:
  void nextState(GameState* state);

 protected:
  ChessGame* game;
};
#endif // include guards
