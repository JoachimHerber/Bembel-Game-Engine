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

class ChessGame;
class ChessPiece;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

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

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
