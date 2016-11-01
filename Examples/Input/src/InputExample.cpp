/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "InputExample.h"

#include <BembelKernel/Kernel.h>
#include <BembelKernel/Display/Window.h>
#include <BembelKernel/Display/DisplayManager.h>

#include <iostream>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

InputExample::InputExample()
	: bembel::Application()
{
	_interactionSys = std::make_shared<bembel::InteractionSystem>(_kernel.get());
	_kernel->AddSystem(_interactionSys);

	_kernel->GetEventManager()->AddHandler<bembel::WindowShouldCloseEvent>(this);

	_kernel->GetEventManager()->AddHandler<bembel::KeyPressEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::KeyRepeatEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::KeyReleaseEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::TextInputEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::MouseButtonPressEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::MouseButtonRepeatEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::MouseButtonReleaseEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::CursorMovedEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::CursorEnteredEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::CursorLeftEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::ScrollEvent>(this);

	_kernel->GetEventManager()->AddHandler<bembel::ButtonPressEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::ButtonReleaseEvent>(this);
}

InputExample::~InputExample()
{}


bool InputExample::Init()
{
	auto window = _kernel->GetDisplayManager()->CreateWindow();
	window->Open("Input Example");
	_kernel->InitSystems();
	return true;
}

void InputExample::Cleanup()
{
	_kernel->ShutdownSystems();
	_kernel->GetDisplayManager()->CloseOpenWindows();
}

void InputExample::Update(double time)
{}

void InputExample::HandleEvent(const WindowShouldCloseEvent& event)
{
	Quit();
}

void InputExample::HandleEvent(const KeyPressEvent& event)
{
	std::cout << "[Key Pressed] " << event.keyID << "|" << event.scancode << std::endl;
}

void InputExample::HandleEvent(const KeyRepeatEvent& event)
{
	std::cout << "[Key Repeated] " << event.keyID << "|" << event.scancode << std::endl;
}

void InputExample::HandleEvent(const KeyReleaseEvent& event)
{
	std::cout << "[Key Released] " << event.keyID << "|" << event.scancode << std::endl;
}

void InputExample::HandleEvent(const TextInputEvent& event)
{
	std::cout << "[TextInput] '" << (unsigned char)(event.character) << "'" << std::endl;
}

void InputExample::HandleEvent(const MouseButtonPressEvent& event)
{
	std::cout << "[Mouse Button Pressed] " << event.buttonID  << std::endl;
}

void InputExample::HandleEvent(const MouseButtonRepeatEvent& event)
{
	std::cout << "[Mouse Button Repeated] " << event.buttonID  << std::endl;
}

void InputExample::HandleEvent(const MouseButtonReleaseEvent& event)
{
	std::cout << "[Mouse Button Released] " << event.buttonID  << std::endl;
}

void InputExample::HandleEvent(const CursorMovedEvent& event)
{
	std::cout << "[Cursor Moved] (" << event.position.x << ";"<< event.position.y << ")" << std::endl;
}

void InputExample::HandleEvent(const CursorEnteredEvent& event)
{
	std::cout << "[Cursor Entered] window[" << event.window->GetWindowID() << "]" << std::endl;
}

void InputExample::HandleEvent(const CursorLeftEvent& event)
{
	std::cout << "[Cursor Left] window[" << event.window->GetWindowID() << "]" << std::endl;
}

void InputExample::HandleEvent(const ScrollEvent& event)
{
	std::cout << "[Scroll] (" << event.x << ";"<< event.y << ")" << std::endl;
}

void InputExample::HandleEvent(const ButtonPressEvent& event)
{
	std::cout << "[ButtonPress] " << event.button->GetName() << std::endl;
}

void InputExample::HandleEvent(const ButtonReleaseEvent& event)
{
	std::cout << "[ButtonRelease] " << event.button->GetName() << std::endl;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

