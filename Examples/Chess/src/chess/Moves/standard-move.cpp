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

#include "standard-move.h"

#include "../chess-piece.h"
#include "../player.h"

#include <bembel-kernel/scene/scene.h>
#include <bembel-kernel/scene/position-component.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
StandardMove::StandardMove(
	const glm::ivec2& dir,
	unsigned maxDist,
	bool attack, 
	bool move)
	: _direction(dir)
	, _maxDistance(maxDist)
	, _attack(attack)
	, _move(move)
{}
StandardMove::StandardMove(
	const glm::ivec2& dir, bool attack, bool move)
	: _direction(dir)
	, _maxDistance(~0U)
	, _attack(attack)
	, _move(move)
{}

StandardMove::~StandardMove()
{}

void StandardMove::GetPosibleMoveParameter(
	ChessPiece* piece,
	const ChessBoard& board,
	std::vector<int>& params)
{
	unsigned owner = piece->GetOwner();

	for (unsigned u = 1; u <= _maxDistance; ++u)
	{
		glm::ivec2 target = GetTargetPosition(piece, u);

		if (target.x < 0 || target.y < 0 || target.x >= 8 || target.y >= 8 )
			return;

		ChessPiece* other = board[target.x][target.y];
		if (other != nullptr)
		{
			if (_attack && other->GetOwner() != owner)
				params.push_back(u);
			return;
		}
		else if (_move)
		{
			params.push_back(u);
		}
	}
}

glm::vec2 StandardMove::GetTargetPosition(
	ChessPiece* piece, int dist)
{
	glm::ivec2 dir = _direction;
	if( piece->GetOwner() == 1 )
		dir.x *= -1;
	return piece->GetPositon() + dist*dir;
}

bool StandardMove::UpdateMoveAnimation(
	double time, 
	ChessPiece* piece,
	int param)
{
	auto scene = piece->GetScene();

	glm::ivec2 pos = 2*piece->GetPositon();
	glm::ivec2 dir = 2*_direction;
	if( piece->GetOwner() == 1 )
		dir.x *= -1;

	float dist = std::sqrt(float(_direction.x*_direction.x + _direction.y*_direction.y));
	float progress = time / dist;
	float fract = progress-std::floor(progress);
	if (progress > param)
		return true; // animation finished

	auto& entitiyPos = scene->GetComponent<bembel::PositionComponent>(
		piece->GetEntity())->position;
	entitiyPos.x = pos.x + float(progress)*dir.x;
	entitiyPos.z = pos.y + float(progress)*dir.y;
	entitiyPos.y = 4*dist*(fract-fract*fract);

	return false;
}

void StandardMove::StartMove(
	ChessPiece* piece,
	ChessBoard& board,
	int param)
{
	glm::vec2 targetPos = GetTargetPosition(piece, param);
	auto other = board[targetPos.x][targetPos.y];
	if (other)
		other->Kill();
}

void StandardMove::EndeMove(
	ChessPiece* piece, 
	ChessBoard& board, 
	int param)
{
	glm::vec2 pos = piece->GetPositon();
	board[pos.x][pos.y] = nullptr;

	glm::vec2 targetPos = GetTargetPosition(piece, param);
	piece->SetPosition(targetPos);
	board[targetPos.x][targetPos.y] = piece;
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
