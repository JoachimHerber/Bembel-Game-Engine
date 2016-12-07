/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Player.h"
#include "ChessPiece.h"
#include "ChessBoard.h"

#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/PositionComponent.h>
/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Player::Player(ChessBoard* board, const std::string& name)
	: _board(board)
	, _name(name)
	, _direction(RIGHT)
{}

ChessBoard* Player::GetChessBoard() const
{
	return _board;
}

void Player::SetMovementDirection(MOVE_DIRECTION dir)
{
	_direction = dir;
}

const std::string& Player::GetName() const
{
	return _name;
}

Player::MOVE_DIRECTION Player::GetMovementDirection() const
{
	return _direction;
}

const std::vector<ChessPiece*>& Player::GetChessPieces() const
{
	return _chessPices;
}

void Player::ClearChessPieces()
{
	_chessPices.clear();
}

void Player::AddChessPiece(ChessPiece* piece)
{
	_chessPices.push_back(piece);
}

void Player::RemoveChessPiece(ChessPiece* piece)
{
	for (size_t n = 0; n < _chessPices.size(); ++n)
	{
		if (_chessPices[n] == piece)
		{
			_chessPices.erase(_chessPices.begin() + n);
			return;
		}
	}
}

glm::ivec2 Player::RotateOffset(const glm::ivec2& offset) const
{
	switch (_direction)
	{
	case bembel::Player::RIGHT:
		return offset;
	case bembel::Player::UP:
		return glm::ivec2(-offset.y, offset.x);
	case bembel::Player::LEFT:
		return -offset;
	case bembel::Player::DOWN:
		return glm::ivec2(offset.y, -offset.x);
		break;
	}
}

void Player::ClearCaptureChessPieces()
{
	_capturedChessPices.clear();
}

void Player::CaptureChessPiece(ChessPiece* piece)
{
	unsigned  row = _capturedChessPices.size() / _captureAreaChessPicesPerRow;
	unsigned  col = _capturedChessPices.size() % _captureAreaChessPicesPerRow;

	_capturedChessPices.push_back(piece);

	auto scene = _board->GetScene();
	auto& entitiyPos = scene->GetComponent<PositionComponent>(
		piece->GetEntity())->position;

	entitiyPos = _captureAreaPos;
	entitiyPos += _captureAreaRowOffset*float(row);
	entitiyPos += _captureAreaCollumOffset*float(col);
}

void Player::SetCaptureArea(
	glm::vec3 pos, 
	glm::vec3 rowOffset,
	glm::vec3 colOffset,
	unsigned rowSize)
{
	_captureAreaPos = pos;
	_captureAreaRowOffset = rowOffset;
	_captureAreaCollumOffset = colOffset;
	_captureAreaChessPicesPerRow = rowSize;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
