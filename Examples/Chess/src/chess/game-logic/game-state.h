#ifndef BEMBEL_GAMESTATE_H
#define BEMBEL_GAMESTATE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
class ChessGame;
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class GameState
{
public:
	GameState( ChessGame* game );
	virtual ~GameState();

	virtual void OnEnterState(){};
	virtual void OnExitState(){};

	virtual void Update(double time){};

protected:
	void NextState(GameState* state);

protected:
	ChessGame* _game;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
