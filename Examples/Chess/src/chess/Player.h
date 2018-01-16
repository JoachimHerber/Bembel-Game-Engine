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

#ifndef BEMBEL_PLAYER_H
#define BEMBEL_PLAYER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <string>
#include <vector>

#include <glm/glm.hpp>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/

class ChessGame;
class ChessPiece;

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class Player
{
public:

	Player( ChessGame*, const std::string& name);

	ChessGame* GetChessGame() const;
	const std::string& GetName() const;

	const std::vector<ChessPiece*>& GetChessPieces() const;
	
	void ClearChessPieces();
	void AddChessPiece(ChessPiece*);
	void RemoveChessPiece(ChessPiece*);

	void ClearCaptureChessPieces();
	void CaptureChessPiece(ChessPiece*);

private:
	ChessGame*     _game;
	std::string    _name;

	std::vector<ChessPiece*> _chessPices;
	std::vector<ChessPiece*> _capturedChessPices;

	glm::vec3 _captureAreaPos;
	glm::vec3 _captureAreaRowOffset;
	glm::vec3 _captureAreaCollumOffset;
	unsigned  _captureAreaChessPicesPerRow;

};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
