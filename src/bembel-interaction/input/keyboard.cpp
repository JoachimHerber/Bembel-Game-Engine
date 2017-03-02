/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "keyboard.h"

#include <GLFW/glfw3.h>

/*============================================================================*/
/* IMPLEMENTATION			                                                  */
/*============================================================================*/
namespace bembel{


Keyboard::Keyboard(EventManager* eventMgr)
	: InputDevice(eventMgr, "Keyboard")
{
	_eventMgr->AddHandler<KeyPressEvent>(this);
	_eventMgr->AddHandler<KeyReleaseEvent>(this);
}

Keyboard::~Keyboard()
{
	_eventMgr->RemoveHandler<KeyPressEvent>(this);
	_eventMgr->RemoveHandler<KeyReleaseEvent>(this);
}

Button* Keyboard::GetKey(int keyID, int scancode)
{
	if (keyID != GLFW_KEY_UNKNOWN)
		scancode = 0;

	auto key = std::make_pair(keyID, scancode);
	auto it = _keyMapping.find(key);

	if (it != _keyMapping.end())
		return _keys[it->second].get();

	unsigned index = _keys.size();

	const char* keyName = glfwGetKeyName(keyID, scancode);
	if (keyName != nullptr)
	{
		_keys.push_back(std::make_shared<Button>(keyName));
	}
	else
	{
		char name[256];
		sprintf(name, "UnkownKey%d", scancode);
		_keys.push_back(std::make_shared<Button>(name));
	}
	_keyMapping.emplace(key, index);

	return _keys.back().get();
}

void Keyboard::HandleEvent(const KeyPressEvent& event)
{
	auto key = GetKey(event.keyID, event.scancode);
	_eventMgr->Broadcast(ButtonPressEvent{key});
	key->SetIsPressed(true);
}

void Keyboard::HandleEvent(const KeyReleaseEvent& event)
{
	auto key = GetKey(event.keyID, event.scancode);
	_eventMgr->Broadcast(ButtonReleaseEvent{key});
	key->SetIsPressed(false);
}

}//end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
