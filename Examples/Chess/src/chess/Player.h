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
namespace bembel{

class ChessBoard;
class ChessPiece;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class Player
{
public:
	enum MOVE_DIRECTION
	{
		RIGHT = 0,
		UP    = 1,
		LEFT  = 2,
		DOWN  = 3
	};

	Player(ChessBoard*, const std::string& name);

	ChessBoard* GetChessBoard() const;

	void SetMovementDirection(MOVE_DIRECTION);
	MOVE_DIRECTION GetMovementDirection() const;
	const std::string& GetName() const;

	const std::vector<ChessPiece*>& GetChessPieces() const;
	
	void ClearChessPieces();
	void AddChessPiece(ChessPiece*);
	void RemoveChessPiece(ChessPiece*);

	glm::ivec2 RotateOffset(const glm::ivec2&) const;

	void ClearCaptureChessPieces();
	void CaptureChessPiece(ChessPiece*);

	void SetCaptureArea(glm::vec3, glm::vec3, glm::vec3, unsigned);

private:
	ChessBoard*    _board;
	std::string    _name;
	MOVE_DIRECTION _direction;

	std::vector<ChessPiece*> _chessPices;
	std::vector<ChessPiece*> _capturedChessPices;

	glm::vec3 _captureAreaPos;
	glm::vec3 _captureAreaRowOffset;
	glm::vec3 _captureAreaCollumOffset;
	unsigned  _captureAreaChessPicesPerRow;

};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
