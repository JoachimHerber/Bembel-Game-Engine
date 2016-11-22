/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "MoveSet.h"
#include "StandardMove.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

ChessPieceMoveSet::ChessPieceMoveSet()
{}

ChessPieceMoveSet::~ChessPieceMoveSet()
{}

void ChessPieceMoveSet::AddMove(
	const glm::ivec2& dir, 
	unsigned maxDist, 
	bool attack /*= true*/, 
	bool move /*= true*/)
{
	AddMoveTemplate(
		std::make_shared<StandardMove>(
			dir, maxDist, attack, move));
}

void ChessPieceMoveSet::AddMove(
	const glm::ivec2& dir,
	bool attack /*= true*/,
	bool move /*= true*/)
{
	AddMoveTemplate(
		std::make_shared<StandardMove>(
			dir, attack, move));
}

void ChessPieceMoveSet::AddMoveTemplate(
	std::shared_ptr<MoveTemplate> move)
{
	_moves.push_back(move);
}

void ChessPieceMoveSet::GetAvailableMoves(
	ChessPiece* chessPice, 
	ChessBoard* board, 
	std::vector<Move>& moves)
{
	for (auto& move : _moves)
	{
		std::vector<glm::ivec2> positions;
		move->GetAvailableTargetPositions(chessPice, board, positions);

		for (const auto& pos : positions)
		{
			moves.push_back(Move{pos, move.get()});
		}
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
