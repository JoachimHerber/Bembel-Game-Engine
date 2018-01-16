/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

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
