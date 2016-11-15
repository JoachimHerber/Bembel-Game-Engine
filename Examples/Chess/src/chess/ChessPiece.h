#ifndef BEMBEL_CHESSPIECE_H
#define BEMBEL_CHESSPIECE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelKernel/Scene/Scene.h>

#include <glm/glm.hpp>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Player;
class ChessPieceType;
class Scene;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class ChessPiece
{
public:
	ChessPiece(Scene*, ChessPieceType*, Player*, const glm::ivec2&);

	void Promote(ChessPieceType*);

	ChessPieceType* GetType() const;
	Player*         GetOwner() const;

	const glm::vec2& GetPositon() const;
	void SetPosition(const glm::vec2& pos);

	bool IsAlive() const;
	void Kill();
	void Reset();

private:
	Scene* _scene;

	ChessPieceType* _type;
	ChessPieceType* _originalType;
	Player*         _owner;
	glm::vec2      _position;
	glm::vec2      _startPositon;

	bool            _isAlive;

	Scene::EntityID _entity;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
