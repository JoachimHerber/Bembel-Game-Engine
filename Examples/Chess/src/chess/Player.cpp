/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "player.h"
#include "chess-piece.h"
#include "chess-game.h"

#include <bembel-kernel/scene/scene.h>
#include <bembel-kernel/scene/position-component.h>
/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
Player::Player(ChessGame* game, const std::string& name)
	: _game(game)
	, _name(name)
{}

ChessGame* Player::GetChessGame() const
{
	return _game;
}

const std::string & Player::GetName() const
{
	return _name;
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

void Player::ClearCaptureChessPieces()
{
	_capturedChessPices.clear();
}

void Player::CaptureChessPiece(ChessPiece* piece)
{
	unsigned  row = _capturedChessPices.size() / _captureAreaChessPicesPerRow;
	unsigned  col = _capturedChessPices.size() % _captureAreaChessPicesPerRow;

	_capturedChessPices.push_back(piece);

	auto scene = _game->GetScene();
	auto& entitiyPos = scene->GetComponent<bembel::PositionComponent>(
		piece->GetEntity())->position;

	entitiyPos = _captureAreaPos;
	entitiyPos += _captureAreaRowOffset*float(row);
	entitiyPos += _captureAreaCollumOffset*float(col);
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
