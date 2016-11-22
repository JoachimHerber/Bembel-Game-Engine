#ifndef BEMBEL_CHESSBOARD_H
#define BEMBEL_CHESSBOARD_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelKernel/Scene/Scene.h>

#include "Player.h"

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Player;
class ChessPieceType;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class ChessBoard
{
public:
	ChessBoard(
		std::shared_ptr<AssetManager>, unsigned w, unsigned h, 
		const std::vector<std::string>& playerNames = {"white","black"});
	~ChessBoard();

	std::shared_ptr<Scene> GetScene();

	unsigned GetWidth() const;
	unsigned GetHeight() const;
	unsigned GetNumPlayer() const;

	Player* GetPlayer(unsigned playerID);

	enum
	{
		PAWN = 0,
		ROOK,
		KNIGHT,
		BISHOP,
		QUEEN,
		KING
	};
	void AddChessPiece(const glm::vec2& pos, unsigned type, unsigned owner);

	void ResetChessBoard();

	void DisableTile(unsigned u, unsigned v);

	void UpdatePossibleMoves();

	bool IsPositionValid(const glm::ivec2&);
	ChessPiece* GetChessPieceAt(const glm::ivec2&);
	Scene::EntityID GetTileEntity(const glm::ivec2&) const;

private:
	void InitTiles();
	void InitDefauldChessPieceTypes();

	std::unique_ptr<ChessPieceType> CreateChessPieceType(const std::string&);

private:
	struct Tile
	{
		Scene::EntityID entity;
		bool            disabled;
	};

	std::shared_ptr<Scene> _scene;

	unsigned _width;
	unsigned _height;

	std::vector<Player> _player;

	std::vector<std::unique_ptr<ChessPieceType>> _chessPieceTypes;
	std::vector<std::unique_ptr<ChessPiece>>     _chessPieces;
	std::vector<std::vector<Tile>>               _tiles;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
