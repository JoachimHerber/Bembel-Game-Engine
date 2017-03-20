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

	// register names for non printable keys
	InitKey( GLFW_KEY_SPACE, "[SPACE]");
	InitKey( GLFW_KEY_ESCAPE, "[ESC]" );
	InitKey( GLFW_KEY_ENTER, "[ENTER]" );
	InitKey( GLFW_KEY_TAB, "[TAB]" );
	InitKey( GLFW_KEY_BACKSPACE, "[BACKSPACE]" );
	InitKey( GLFW_KEY_INSERT, "[INSERT]" );
	InitKey( GLFW_KEY_DELETE, "[DELETE]" );
	InitKey( GLFW_KEY_RIGHT, "[RIGHT]" );
	InitKey( GLFW_KEY_LEFT, "[LEFT]" );
	InitKey( GLFW_KEY_DOWN, "[DOWN]" );
	InitKey( GLFW_KEY_UP, "[UP]" );
	InitKey( GLFW_KEY_PAGE_UP, "[PAGE_UP]" );
	InitKey( GLFW_KEY_PAGE_DOWN, "[PAGE_DOWN]" );
	InitKey( GLFW_KEY_HOME, "[PAGE_HOME]" );
	InitKey( GLFW_KEY_END, "[END]" );
	InitKey( GLFW_KEY_CAPS_LOCK, "[CAPS_LOCK]" );
	InitKey( GLFW_KEY_SCROLL_LOCK, "[SCROLL_LOCK]" );
	InitKey( GLFW_KEY_NUM_LOCK, "[NUM_LOCK]" );
	InitKey( GLFW_KEY_PRINT_SCREEN, "[PRINT]" );
	InitKey( GLFW_KEY_PAUSE, "[PAUSE]" );
	InitKey( GLFW_KEY_F1, "[F1]" );
	InitKey( GLFW_KEY_F2, "[F2]" );
	InitKey( GLFW_KEY_F3, "[F3]" );
	InitKey( GLFW_KEY_F4, "[F4]" );
	InitKey( GLFW_KEY_F5, "[F5]" );
	InitKey( GLFW_KEY_F6, "[F6]" );
	InitKey( GLFW_KEY_F7, "[F7]" );
	InitKey( GLFW_KEY_F8, "[F8]" );
	InitKey( GLFW_KEY_F9, "[F9]" );
	InitKey( GLFW_KEY_F10, "[F10]" );
	InitKey( GLFW_KEY_F11, "[F11]" );
	InitKey( GLFW_KEY_F12, "[F12]" );
	InitKey( GLFW_KEY_F13, "[F13]" );
	InitKey( GLFW_KEY_F14, "[F14]" );
	InitKey( GLFW_KEY_F15, "[F15]" );
	InitKey( GLFW_KEY_F16, "[F16]" );
	InitKey( GLFW_KEY_F17, "[F17]" );
	InitKey( GLFW_KEY_F18, "[F18]" );
	InitKey( GLFW_KEY_F19, "[F19]" );
	InitKey( GLFW_KEY_F20, "[F20]" );
	InitKey( GLFW_KEY_F21, "[F21]" );
	InitKey( GLFW_KEY_F22, "[F22]" );
	InitKey( GLFW_KEY_F23, "[F23]" );
	InitKey( GLFW_KEY_F24, "[F24]" );
	InitKey( GLFW_KEY_F25, "[F25]" );
	InitKey( GLFW_KEY_KP_0, "[NUM_PAD_0]" );
	InitKey( GLFW_KEY_KP_1, "[NUM_PAD_1]" );
	InitKey( GLFW_KEY_KP_2, "[NUM_PAD_2]" );
	InitKey( GLFW_KEY_KP_3, "[NUM_PAD_3]" );
	InitKey( GLFW_KEY_KP_4, "[NUM_PAD_4]" );
	InitKey( GLFW_KEY_KP_5, "[NUM_PAD_5]" );
	InitKey( GLFW_KEY_KP_6, "[NUM_PAD_6]" );
	InitKey( GLFW_KEY_KP_7, "[NUM_PAD_7]" );
	InitKey( GLFW_KEY_KP_8, "[NUM_PAD_8]" );
	InitKey( GLFW_KEY_KP_9, "[NUM_PAD_9]" );
	InitKey( GLFW_KEY_KP_DECIMAL, "[NUM_PAD_DECIMAL]" );
	InitKey( GLFW_KEY_KP_DIVIDE, "[NUM_PAD_DIVIDE]" );
	InitKey( GLFW_KEY_KP_MULTIPLY, "[NUM_PAD_MULTIPLY]" );
	InitKey( GLFW_KEY_KP_SUBTRACT, "[NUM_PAD_SUBTRACT]" );
	InitKey( GLFW_KEY_KP_ADD, "[NUM_PAD_ADD]" );
	InitKey( GLFW_KEY_KP_ENTER, "[NUM_PAD_ENTER]" );
	InitKey( GLFW_KEY_KP_EQUAL, "[NUM_PAD_EQUAL]" );
	InitKey( GLFW_KEY_LEFT_SHIFT, "[SHIFT]" );
	InitKey( GLFW_KEY_LEFT_CONTROL, "[CONTROL]" );
	InitKey( GLFW_KEY_LEFT_ALT, "[ALT]" );
	InitKey( GLFW_KEY_RIGHT_SHIFT, "[RIGHT_SHIFT]" );
	InitKey( GLFW_KEY_RIGHT_CONTROL, "[RIGHT_CONTROL]" );
	InitKey( GLFW_KEY_RIGHT_ALT, "[RIGHT_ALT]" );
	InitKey( GLFW_KEY_MENU, "[MENU]" );
}

Keyboard::~Keyboard()
{
	_eventMgr->RemoveHandler<KeyPressEvent>(this);
	_eventMgr->RemoveHandler<KeyReleaseEvent>(this);
}

Button* Keyboard::GetKey(int key_id, int scancode)
{
	if (key_id != GLFW_KEY_UNKNOWN)
		scancode = 0;

	auto key = std::make_pair(key_id, scancode);
	auto it = _keyMapping.find(key);

	if (it != _keyMapping.end())
		return _keys[it->second].get();

	unsigned index = _keys.size();

	const char* keyName = glfwGetKeyName(key_id, scancode);
	if (keyName != nullptr)
	{
		_keys.push_back(std::make_unique<Button>(keyName));
	}
	else
	{
		char name[256];
		sprintf(name, "[KEY%d]", scancode);
		_keys.push_back(std::make_unique<Button>(name));
	}
	_keyMapping.emplace(key, index);

	return _keys.back().get();
}

void Keyboard::HandleEvent(const KeyPressEvent& event)
{
	auto key = GetKey(event.key_id, event.scancode);
	_eventMgr->Broadcast(ButtonPressEvent{key});
	key->SetIsPressed(true);
}

void Keyboard::HandleEvent(const KeyReleaseEvent& event)
{
	auto key = GetKey(event.key_id, event.scancode);
	_eventMgr->Broadcast(ButtonReleaseEvent{key});
	key->SetIsPressed(false);
}

void Keyboard::InitKey( int key_id, const std::string & name )
{
	auto key = std::make_pair( key_id, 0 );
	auto it = _keyMapping.find( key );

	unsigned index = _keys.size();

	_keys.push_back( std::make_unique<Button>( name ) );
	_keyMapping.emplace( key, index );

}

}//end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
