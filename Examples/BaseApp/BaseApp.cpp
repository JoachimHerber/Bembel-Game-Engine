/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "BaseApp.h"

#include <BembelOpenGL.h>

#include <BembelKernel/Kernel.h>
#include <BembelKernel/Display/DisplayManager.h>

#include <chrono>
/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/

BaseApp::BaseApp()
{
	_kernel->GetEventManager()->AddHandler<bembel::WindowShouldCloseEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::WindowUpdateEvent>(this);
}

BaseApp::~BaseApp()
{}

bool BaseApp::Init()
{
	_kernel->GetDisplayManager()->CreateWindow()->Open("BaseApp");
	return true;
}

void BaseApp::Cleanup()
{
	
}

void BaseApp::Update(double timeDelta)
{
	
}

void BaseApp::HandleEvent(const bembel::WindowShouldCloseEvent& event)
{
	Quit();
}

void BaseApp::HandleEvent(const bembel::WindowUpdateEvent& event)
{
	using namespace std::chrono;

	static high_resolution_clock::time_point startTime = high_resolution_clock::now();
	high_resolution_clock::time_point time = high_resolution_clock::now();

	const static double twoPiThirds = 2*3.14159265359/3;
	double gameTime = duration_cast<duration<double>>(time-startTime).count();

	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(0.8f*sin(gameTime+0*twoPiThirds), 0.8f*cos(gameTime+0*twoPiThirds), +0.0f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.8f*sin(gameTime+1*twoPiThirds), 0.8f*cos(gameTime+1*twoPiThirds), +0.0f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.8f*sin(gameTime+2*twoPiThirds), 0.8f*cos(gameTime+2*twoPiThirds), +0.0f);
	glEnd();
}

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

