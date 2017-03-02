/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "MoveSet.h"
#include "StandardMove.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

MoveSet::MoveSet()
{}

MoveSet::~MoveSet()
{}

void MoveSet::AddMove(
	const glm::ivec2& dir, 
	unsigned maxDist, 
	bool attack /*= true*/, 
	bool move /*= true*/)
{
	AddMoveTemplate(
		std::make_shared<StandardMove>(
			dir, maxDist, attack, move));
}

void MoveSet::AddMove(
	const glm::ivec2& dir,
	bool attack /*= true*/,
	bool move /*= true*/)
{
	AddMoveTemplate(
		std::make_shared<StandardMove>(
			dir, attack, move));
}

void MoveSet::AddMoveTemplate(
	std::shared_ptr<MoveTemplate> move)
{
	_moves.push_back(move);
}

void MoveSet::GetAvailableMoves(
	ChessPiece* chessPice, 
	const ChessBoard& board,
	std::vector<Move>& moves)
{
	for (auto& move : _moves)
	{
		std::vector<int> params;
		move->GetPosibleMoveParameter(
			chessPice, board, params);

		for (const auto& it : params)
		{
			moves.push_back(Move{move.get(), it});
		}
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
