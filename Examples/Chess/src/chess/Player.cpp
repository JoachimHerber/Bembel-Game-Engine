/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Player.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Player::Player(const std::string& name)
	: _name(name)
	, _direction(RIGHT)
{}

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

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
