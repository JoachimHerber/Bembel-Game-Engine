/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "ConsoleLogSink.h"
#include "LogMessage.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

ConsoleLogSink::ConsoleLogSink(const std::string& prefix, int color)
	: _prefix(prefix)
	, _color(color)
{}

ConsoleLogSink::~ConsoleLogSink()
{}

void ConsoleLogSink::WriteMessage(const LogMessage&msg)
{
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _color);
#endif

	std::cout << _prefix << msg.GetDescription() << std::flush;

#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
#endif
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
