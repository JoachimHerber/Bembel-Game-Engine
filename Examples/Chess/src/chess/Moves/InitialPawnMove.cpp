/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "InitialPawnMove.h"

#include "../ChessPiece.h"
#include "../ChessBoard.h"
#include "../Player.h"

#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/PositionComponent.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

InitialPawnMove::InitialPawnMove()
{}

InitialPawnMove::~InitialPawnMove()
{}

void InitialPawnMove::GetPosibleMoveParameter(
	ChessPiece* piece, std::vector<int>& params)
{
	if (piece->HasMoved())
		return;
	Player* owner = piece->GetOwner();
	auto p1 = piece->GetPositon() + owner->RotateOffset({1,0});
	auto p2 = piece->GetPositon() + owner->RotateOffset({2,0});

	if (piece->GetBoard()->GetChessPieceAt(p1) || 
		piece->GetBoard()->GetChessPieceAt(p2))
	{
		return;
	}
	params.push_back(0);
}

glm::vec2 InitialPawnMove::GetTargetPosition(
	ChessPiece* piece, int)
{
	Player* owner = piece->GetOwner();
	return piece->GetPositon() +
		owner->RotateOffset({2,0});
}

void InitialPawnMove::StartMove(ChessPiece*, int)
{
	
}

void InitialPawnMove::EndeMove(ChessPiece* piece, int)
{
	glm::vec2 targetPos = GetTargetPosition(piece, 0);
	piece->SetPosition(targetPos);
}

bool InitialPawnMove::UpdateMoveAnimation(double time, ChessPiece* piece, int)
{
	auto scene = piece->GetBoard()->GetScene();

	glm::ivec2 pos = 2*piece->GetPositon();
	glm::ivec2 dir = 2*piece->GetOwner()->RotateOffset({2,0});

	float progress = time / 2.0f;
	float fract = progress-std::floor(progress);
	if (progress > 1)
		return true; // animation finished

	auto& entitiyPos = scene->GetComponent<PositionComponent>(
		piece->GetEntity())->position;
	entitiyPos.x = pos.x + float(progress)*dir.x;
	entitiyPos.z = pos.y + float(progress)*dir.y;
	entitiyPos.y = 4*(fract-fract*fract);

	return false;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
