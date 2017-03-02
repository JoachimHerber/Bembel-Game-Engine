#ifndef BEMBEL_CHESSPIECEMOVESET_H
#define BEMBEL_CHESSPIECEMOVESET_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <glm/glm.hpp>

#include "../ChessGame.h"

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class ChessPiece;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class MoveSet
{
public:
	class MoveTemplate
	{
	public:
		MoveTemplate(){};
		virtual ~MoveTemplate(){};

		virtual void GetPosibleMoveParameter(ChessPiece*, const ChessBoard&, std::vector<int>&) = 0;
		virtual glm::vec2 GetTargetPosition(ChessPiece*, int) = 0;

		virtual void StartMove(ChessPiece*, ChessBoard&, int) = 0;
		virtual void EndeMove(ChessPiece*, ChessBoard&, int) = 0;

		virtual bool UpdateMoveAnimation(double, ChessPiece*, int) = 0;
	};

	struct Move
	{
		MoveTemplate* move;
		int           param;
	};

public:
	MoveSet();
	~MoveSet();

	void AddMove(const glm::ivec2& dir, 
				 unsigned maxDist,
				 bool attack = true,
				 bool move = true);
	void AddMove(const glm::ivec2& dir,
				 bool attack = true,
				 bool move = true);

	void AddMoveTemplate(std::shared_ptr<MoveTemplate>);

	void GetAvailableMoves(ChessPiece*, const ChessBoard&, std::vector<Move>&);

private:
	std::vector<std::shared_ptr<MoveTemplate>> _moves;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
