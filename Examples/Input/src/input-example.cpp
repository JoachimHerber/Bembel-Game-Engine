/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "input-example.h"

#include <bembel-kernel/kernel.h>
#include <bembel-kernel/display/window.h>
#include <bembel-kernel/display/display-manager.h>

#include <iostream>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
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

void InputExample::HandleEvent(const bembel::WindowShouldCloseEvent& event)
{
	Quit();
}

void InputExample::HandleEvent(const bembel::KeyPressEvent& event)
{
	std::cout << "[Key Pressed] " << event.keyID << "|" << event.scancode << std::endl;
}

void InputExample::HandleEvent(const bembel::KeyRepeatEvent& event)
{
	std::cout << "[Key Repeated] " << event.keyID << "|" << event.scancode << std::endl;
}

void InputExample::HandleEvent(const bembel::KeyReleaseEvent& event)
{
	std::cout << "[Key Released] " << event.keyID << "|" << event.scancode << std::endl;
}

void InputExample::HandleEvent(const bembel::TextInputEvent& event)
{
	std::cout << "[TextInput] '" << (unsigned char)(event.character) << "'" << std::endl;
}

void InputExample::HandleEvent(const bembel::MouseButtonPressEvent& event)
{
	std::cout << "[Mouse Button Pressed] " << event.buttonID  << std::endl;
}

void InputExample::HandleEvent(const bembel::MouseButtonRepeatEvent& event)
{
	std::cout << "[Mouse Button Repeated] " << event.buttonID  << std::endl;
}

void InputExample::HandleEvent(const bembel::MouseButtonReleaseEvent& event)
{
	std::cout << "[Mouse Button Released] " << event.buttonID  << std::endl;
}

void InputExample::HandleEvent(const bembel::CursorMovedEvent& event)
{
	std::cout << "[Cursor Moved] (" << event.position.x << ";"<< event.position.y << ")" << std::endl;
}

void InputExample::HandleEvent(const bembel::CursorEnteredEvent& event)
{
	std::cout << "[Cursor Entered] window[" << event.window->GetWindowID() << "]" << std::endl;
}

void InputExample::HandleEvent(const bembel::CursorLeftEvent& event)
{
	std::cout << "[Cursor Left] window[" << event.window->GetWindowID() << "]" << std::endl;
}

void InputExample::HandleEvent(const bembel::ScrollEvent& event)
{
	std::cout << "[Scroll] (" << event.x << ";"<< event.y << ")" << std::endl;
}

void InputExample::HandleEvent(const bembel::ButtonPressEvent& event)
{
	std::cout << "[ButtonPress] " << event.button->GetName() << std::endl;
}

void InputExample::HandleEvent(const bembel::ButtonReleaseEvent& event)
{
	std::cout << "[ButtonRelease] " << event.button->GetName() << std::endl;
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

