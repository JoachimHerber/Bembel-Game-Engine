#ifndef BEMBEL_SELECTCHESSBOARDSTATE_H
#define BEMBEL_SELECTCHESSBOARDSTATE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GameState.h"

#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class CameraControle;
class RenderingPipeline;
class ChessBoard;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class SelectChessBoardState : public GameState
{
public:
	SelectChessBoardState(StateMashine*, RenderingPipeline*, CameraControle*);
	~SelectChessBoardState();

	void AddChessBoard(ChessBoard* board, GameState* state);

	virtual void OnNextButtonPress() override;
	virtual void OnPrevButtonPress() override;
	virtual void OnSelectButtonPress() override;

	virtual void OnEnterState() override;
	virtual void OnExitState() override;

	virtual void Update(double time) override;

private:
	void OnBoardChange();

private:
	RenderingPipeline* _pipline;
	CameraControle*    _cam;

	std::vector<ChessBoard*> _boards;
	std::vector<GameState*>  _states;

	unsigned _selection = 0;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards

