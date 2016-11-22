#ifndef BEMBEL_CHESSPIECEMOVESET_H
#define BEMBEL_CHESSPIECEMOVESET_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <glm/glm.hpp>

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class ChessPiece;
class ChessBoard;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class ChessPieceMoveSet
{
public:
	class MoveTemplate
	{
	public:
		MoveTemplate(){};
		virtual ~MoveTemplate(){};

		virtual void GetAvailableTargetPositions(ChessPiece*, ChessBoard*, std::vector<glm::ivec2>&) = 0;
	};

	struct Move
	{
		glm::ivec2    target;
		MoveTemplate* move;
	};

public:
	ChessPieceMoveSet();
	~ChessPieceMoveSet();

	void AddMove(const glm::ivec2& dir, 
				 unsigned maxDist,
				 bool attack = true,
				 bool move = true);
	void AddMove(const glm::ivec2& dir,
				 bool attack = true,
				 bool move = true);

	void AddMoveTemplate(std::shared_ptr<MoveTemplate>);

	void GetAvailableMoves(ChessPiece*, ChessBoard*, std::vector<Move>&);

private:
	std::vector<std::shared_ptr<MoveTemplate>> _moves;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
