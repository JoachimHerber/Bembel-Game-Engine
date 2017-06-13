#ifndef BEMBEL_CHESSGAME_H
#define BEMBEL_CHESSGAME_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <bembel-kernel/scene/scene.h>

#include "player.h"

#include <memory>
#include <vector>
#include <array>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/

class SelectionComponent;
class SelectionPointer;
class GameState;
class ChessPieceType;

namespace bembel{

class EventManager;
class GraphicSystem;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/

using ChessBoard = std::array<std::array<ChessPiece*, 8>, 8>;

class ChessGame
{
public:
	ChessGame( bembel::AssetManager*, bembel::EventManager*, bembel::GraphicSystem*);
	~ChessGame();

	std::shared_ptr<bembel::Scene> GetScene() const;

	ChessBoard& GetChessBoard();
	const ChessBoard& GetChessBoard() const;

	const std::array<Player, 2>& GetPlayer() const;

	enum
	{
		PAWN = 0,
		ROOK,
		KNIGHT,
		BISHOP,
		QUEEN,
		KING
	};

	void ResetChessBoard();
	void UpdatePossibleMoves();

	SelectionComponent* GetBoardTileSelectionComponent(const glm::ivec2&);

	void Update( double time );
	void SetNextState( GameState* state );

private:

	void InitTiles();
	void InitChessPieceTypes();
	void InitChessPices();
	void InitStates();

	std::unique_ptr<ChessPieceType> CreateChessPieceType( const std::string& );
	void AddChessPiece( const glm::vec2& pos, unsigned type, unsigned owner );

private:
	std::shared_ptr<bembel::Scene> scene_;

	std::unique_ptr<SelectionPointer> _selectionPointer;

	std::array<Player, 2> _player;

	std::vector<std::unique_ptr<ChessPieceType>> _chessPieceTypes;
	std::vector<std::unique_ptr<ChessPiece>>     _chessPieces;
	
	ChessBoard _board;

	std::array<std::array<bembel::Scene::EntityID, 8>, 8> _tiles;

	std::vector<std::unique_ptr<GameState>> _states;

	GameState* _currentState = nullptr;
	GameState* _nextState = nullptr;
};

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
