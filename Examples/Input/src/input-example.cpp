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
	kernel_->AddSystem<bembel::InteractionSystem>();

	kernel_->GetEventManager()->AddHandler<bembel::WindowShouldCloseEvent>(this);

	kernel_->GetEventManager()->AddHandler<bembel::KeyPressEvent>(this);
	kernel_->GetEventManager()->AddHandler<bembel::KeyRepeatEvent>(this);
	kernel_->GetEventManager()->AddHandler<bembel::KeyReleaseEvent>(this);
	kernel_->GetEventManager()->AddHandler<bembel::TextInputEvent>(this);
	kernel_->GetEventManager()->AddHandler<bembel::MouseButtonPressEvent>(this);
	kernel_->GetEventManager()->AddHandler<bembel::MouseButtonRepeatEvent>(this);
	kernel_->GetEventManager()->AddHandler<bembel::MouseButtonReleaseEvent>(this);
	kernel_->GetEventManager()->AddHandler<bembel::CursorMovedEvent>(this);
	kernel_->GetEventManager()->AddHandler<bembel::CursorEnteredEvent>(this);
	kernel_->GetEventManager()->AddHandler<bembel::CursorLeftEvent>(this);
	kernel_->GetEventManager()->AddHandler<bembel::ScrollEvent>(this);

	kernel_->GetEventManager()->AddHandler<bembel::ButtonPressEvent>(this);
	kernel_->GetEventManager()->AddHandler<bembel::ButtonReleaseEvent>(this);
}

InputExample::~InputExample()
{}


bool InputExample::Init()
{
	auto window = kernel_->GetDisplayManager()->CreateWindow();
	window->Open("Input Example");
	kernel_->InitSystems();
	return true;
}

void InputExample::Cleanup()
{
	kernel_->ShutdownSystems();
	kernel_->GetDisplayManager()->CloseOpenWindows();
}

void InputExample::Update(double time)
{}

void InputExample::HandleEvent(const bembel::WindowShouldCloseEvent& event)
{
	Quit();
}

void InputExample::HandleEvent(const bembel::KeyPressEvent& event)
{
	std::cout << "[Key Pressed] " << event.key_id << "|" << event.scancode << std::endl;
}

void InputExample::HandleEvent(const bembel::KeyRepeatEvent& event)
{
	std::cout << "[Key Repeated] " << event.key_id << "|" << event.scancode << std::endl;
}

void InputExample::HandleEvent(const bembel::KeyReleaseEvent& event)
{
	std::cout << "[Key Released] " << event.key_id << "|" << event.scancode << std::endl;
}

void InputExample::HandleEvent(const bembel::TextInputEvent& event)
{
	std::cout << "[TextInput] '" << (unsigned char)(event.character) << "'" << std::endl;
}

void InputExample::HandleEvent(const bembel::MouseButtonPressEvent& event)
{
	std::cout << "[Mouse Button Pressed] " << event.button_id  << std::endl;
}

void InputExample::HandleEvent(const bembel::MouseButtonRepeatEvent& event)
{
	std::cout << "[Mouse Button Repeated] " << event.button_id  << std::endl;
}

void InputExample::HandleEvent(const bembel::MouseButtonReleaseEvent& event)
{
	std::cout << "[Mouse Button Released] " << event.button_id  << std::endl;
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

