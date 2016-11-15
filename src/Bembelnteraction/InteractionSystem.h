#ifndef BEMBEL_INTERACTIONSYSTEM_H
#define BEMBEL_INTERACTIONSYSTEM_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <BembelKernel/System.h>

#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Keyboard;
class Mouse;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API InteractionSystem : public System
{
public:
	InteractionSystem(Kernel*);
	~InteractionSystem();

	virtual bool Configure(const xml::Element*) override;

	virtual bool Init() override;
	virtual void Shutdown() override;
	virtual void Update(double timeSinceLastUpdate) override;

	Mouse*    GetMouse()    const;
	Keyboard* GetKeyboard() const;

private:
	std::unique_ptr<Mouse>    _mouse;
	std::unique_ptr<Keyboard> _keyboard;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
