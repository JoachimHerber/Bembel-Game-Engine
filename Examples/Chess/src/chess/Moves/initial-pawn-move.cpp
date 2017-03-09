/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "initial-pawn-move.h"

#include "../chess-piece.h"
#include "../player.h"

#include <bembel-kernel/scene/scene.h>
#include <bembel-kernel/scene/position-component.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
InitialPawnMove::InitialPawnMove()
{}

InitialPawnMove::~InitialPawnMove()
{}

void InitialPawnMove::GetPosibleMoveParameter(
	ChessPiece* piece, const ChessBoard& board, std::vector<int>& params)
{
	if (piece->HasMoved())
		return;

	unsigned owner = piece->GetOwner();
	auto p1 = piece->GetPositon() + glm::ivec2{owner==0?1:-1,0};
	auto p2 = piece->GetPositon() + glm::ivec2{owner==0?2:-2,0};

	if ( board[p1.x][p1.y] != nullptr || board[p2.x][p2.y] != nullptr )
	{
		return;
	}
	params.push_back(0);
}

glm::vec2 InitialPawnMove::GetTargetPosition(
	ChessPiece* piece, int)
{
	unsigned owner = piece->GetOwner();
	return piece->GetPositon() + glm::ivec2{owner==0 ? 2 : -2,0};
}

void InitialPawnMove::StartMove(ChessPiece*, ChessBoard&, int)
{
	
}

void InitialPawnMove::EndeMove(ChessPiece* piece, ChessBoard& board, int)
{
	glm::vec2 pos = piece->GetPositon();
	board[pos.x][pos.y] = nullptr;

	glm::vec2 targetPos = GetTargetPosition(piece, 0);
	piece->SetPosition(targetPos);
	board[targetPos.x][targetPos.y] = piece;
}

bool InitialPawnMove::UpdateMoveAnimation(double time, ChessPiece* piece, int)
{
	auto scene = piece->GetScene();

	glm::ivec2 pos = 2*piece->GetPositon();
	glm::ivec2 dir = 2*glm::ivec2{piece->GetOwner()==0 ? 2 : -2,0};

	float progress = time / 2.0f;
	float fract = progress-std::floor(progress);
	if (progress > 1)
		return true; // animation finished

	auto& entitiyPos = scene->GetComponent<bembel::PositionComponent>(
		piece->GetEntity())->position;
	entitiyPos.x = pos.x + float(progress)*dir.x;
	entitiyPos.z = pos.y + float(progress)*dir.y;
	entitiyPos.y = 4*(fract-fract*fract);

	return false;
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
