/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "console-log-sink.h"
#include "log-message.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

ConsoleLogSink::ConsoleLogSink(const std::string& prefix, int color)
	: prefix_(prefix)
	, color_(color)
{}

ConsoleLogSink::~ConsoleLogSink()
{}

void ConsoleLogSink::WriteMessage(const LogMessage&msg)
{
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_);
#endif

	std::cout << prefix_ << msg.GetDescription() << std::flush;

#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
#endif
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
