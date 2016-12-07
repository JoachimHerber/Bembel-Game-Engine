/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "ChessBoard.h"
#include "ChessPiece.h"
#include "ChessPieceType.h"
#include "Player.h"
#include "Moves/InitialPawnMove.h"

#include <BembelKernel/Assets/AssetManager.h>
#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Scene/GeometryComponent.h>
#include <BembelKernel/Rendering/GeometryModel.h>
#include <BembelGraphics/RenderingPipeline/LightSourceProperties.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

ChessBoard::ChessBoard(
	std::shared_ptr<AssetManager> assetMgr, unsigned w, unsigned h,
	const std::vector<std::string>& playerNames)
	: _scene(std::make_shared<Scene>(assetMgr))
	, _width(w)
	, _height(h)
{
	_scene->RegisterComponentType<PositionComponent>();
	_scene->RegisterComponentType<DirLightProperties>();
	_scene->RegisterComponentType<GeometryComponent>();

	auto entity = _scene->CreateEntity();
	auto light = _scene->CreateComponent<DirLightProperties>(entity);
	light->direction = glm::normalize(glm::vec3(-0.3, -1, -0.2));
	light->color = 3.0f*glm::vec3(1, 1, 1);

	for (const auto& playerName : playerNames)
		_player.push_back(Player(this, playerName));

	InitTiles();
	InitDefauldChessPieceTypes();
}

ChessBoard::~ChessBoard()
{}

std::shared_ptr<Scene> ChessBoard::GetScene()
{
	return _scene;
}

unsigned ChessBoard::GetWidth() const
{
	return _width;
}

unsigned ChessBoard::GetHeight() const
{
	return _height;
}

unsigned ChessBoard::GetNumPlayer() const
{
	return static_cast<unsigned>(_player.size());
}

Player* ChessBoard::GetPlayer(unsigned playerID)
{
	if (playerID <_player.size())
		return &_player[playerID];

	return nullptr;
}

void ChessBoard::AddChessPiece(
	const glm::vec2& pos, 
	unsigned type,
	unsigned owner)
{
	_chessPieces.push_back(std::make_unique<ChessPiece>(
		_chessPieceTypes[type].get(), 
		&_player[owner], 
		pos
	));
}

void ChessBoard::ResetChessBoard()
{
	for (auto& player : _player)
	{
		player.ClearChessPieces();
		player.ClearCaptureChessPieces();
	}

	for (auto& it : _chessPieces)
	{
		it->Reset();
		it->GetOwner()->AddChessPiece(it.get());
	}
}

void ChessBoard::DisableTile(unsigned u, unsigned v)
{
	_scene->DeleteEntity(_tiles[u][v].entity);
	_tiles[u][v].disabled = true;
	_tiles[u][v].entity = Scene::INVALID_ENTITY;
}

void ChessBoard::UpdatePossibleMoves()
{
	for (auto& it : _chessPieces)
		it->UpdatePossibleMoves();
}

bool ChessBoard::IsPositionValid(const glm::ivec2& pos)
{
	if (pos.x < 0 || pos.y < 0 || _width <= pos.x || _height <= pos.y)
		return false;

	return !(_tiles[pos.x][pos.y].disabled);
}

ChessPiece* ChessBoard::GetChessPieceAt(const glm::ivec2& pos)
{
	for (auto& it : _chessPieces)
	{
		if (it->GetPositon() == pos)
			return  it.get();
	}
	return nullptr;
}

Scene::EntityID ChessBoard::GetTileEntity(const glm::ivec2& pos) const
{
	if (pos.x < 0 || pos.y < 0 || _width <= pos.x || _height <= pos.y)
		return Scene::INVALID_ENTITY;

	return _tiles[pos.x][pos.y].entity;
}

void ChessBoard::InitTiles()
{
	AssetHandle whiteTile =
		_scene->GetAssetManager()->GetAssetHandle<GeometryModel>("white.tile");
	AssetHandle blackTile =
		_scene->GetAssetManager()->GetAssetHandle<GeometryModel>("black.tile");

	_tiles.resize(_width);
	for (unsigned u = 0; u < _width; ++u)
	{
		_tiles[u].resize(_height);
		for (unsigned v = 0; v < _height; ++v)
		{
			_tiles[u][v].entity   = _scene->CreateEntity();
			_tiles[u][v].disabled = false;

			auto posComp = _scene->CreateComponent<PositionComponent>(_tiles[u][v].entity);
			posComp->position = glm::vec3(2.0f*u, 0, 2.0f*v);
			auto geomComt = _scene->CreateComponent<GeometryComponent>(_tiles[u][v].entity);
			geomComt->model = ((u+v)%2 == 0 ? whiteTile : blackTile);
		}
	}
}

void ChessBoard::InitDefauldChessPieceTypes()
{
	_chessPieceTypes.resize(6);
	_chessPieceTypes[PAWN]   = CreateChessPieceType("pawn");
	_chessPieceTypes[ROOK]   = CreateChessPieceType("rook");
	_chessPieceTypes[KNIGHT] = CreateChessPieceType("knight");
	_chessPieceTypes[BISHOP] = CreateChessPieceType("bishop");
	_chessPieceTypes[QUEEN]  = CreateChessPieceType("queen");
	_chessPieceTypes[KING]   = CreateChessPieceType("king");

	_chessPieceTypes[PAWN]->AddMove({1,-1}, 1, true, false);
	_chessPieceTypes[PAWN]->AddMove({1,+0}, 1, false, true);
	_chessPieceTypes[PAWN]->AddMove({1,+1}, 1, true, false);
	_chessPieceTypes[PAWN]->GetMoveSet().AddMoveTemplate( 
		std::make_shared<InitialPawnMove>());

	_chessPieceTypes[ROOK]->AddMove({ 1, 0});
	_chessPieceTypes[ROOK]->AddMove({ 0,-1});
	_chessPieceTypes[ROOK]->AddMove({-1, 0});
	_chessPieceTypes[ROOK]->AddMove({ 0,+1});

	_chessPieceTypes[KNIGHT]->AddMove({+1,+2}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({+2,+1}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({+2,-1}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({+1,-2}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({-1,-2}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({-2,-1}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({-2,+1}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({-1,+2}, 1U);

	_chessPieceTypes[BISHOP]->AddMove({+1,+1});
	_chessPieceTypes[BISHOP]->AddMove({+1,-1});
	_chessPieceTypes[BISHOP]->AddMove({-1,-1});
	_chessPieceTypes[BISHOP]->AddMove({-1,+1});

	_chessPieceTypes[QUEEN]->AddMove({+1,+1});
	_chessPieceTypes[QUEEN]->AddMove({+1, 0});
	_chessPieceTypes[QUEEN]->AddMove({+1,-1});
	_chessPieceTypes[QUEEN]->AddMove({ 0,-1});
	_chessPieceTypes[QUEEN]->AddMove({-1,-1});
	_chessPieceTypes[QUEEN]->AddMove({-1, 0});
	_chessPieceTypes[QUEEN]->AddMove({-1,+1});
	_chessPieceTypes[QUEEN]->AddMove({ 0,+1});

	_chessPieceTypes[KING]->AddMove({+1,+1}, 1U);
	_chessPieceTypes[KING]->AddMove({+1, 0}, 1U);
	_chessPieceTypes[KING]->AddMove({+1,-1}, 1U);
	_chessPieceTypes[KING]->AddMove({ 0,-1}, 1U);
	_chessPieceTypes[KING]->AddMove({-1,-1}, 1U);
	_chessPieceTypes[KING]->AddMove({-1, 0}, 1U);
	_chessPieceTypes[KING]->AddMove({-1,+1}, 1U);
	_chessPieceTypes[KING]->AddMove({ 0,+1}, 1U);
}

std::unique_ptr<ChessPieceType> ChessBoard::CreateChessPieceType(
	const std::string& modleSufix)
{
	auto type = std::make_unique<ChessPieceType>();

	for (size_t n = 0; n <_player.size(); ++n)
	{
		type->SetModle(
			&_player[n], 
			_scene->GetAssetManager().get(), 
			_player[n].GetName() + "." + modleSufix
		);
	}
	return std::move(type);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
