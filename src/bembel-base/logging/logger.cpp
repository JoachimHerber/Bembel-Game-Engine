/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "logger.h"
#include "log-sink.h"
#include "console-log-sink.h"


/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Logger Logger::info(std::make_shared<ConsoleLogSink>("", 0x0F));
Logger Logger::error(std::make_shared<ConsoleLogSink>("###ERROR###\t", 0xCE));
Logger Logger::debug(std::make_shared<ConsoleLogSink>("", 0x09));
Logger Logger::warning(std::make_shared<ConsoleLogSink>("- WARNING -\t", 0xE0));

Logger::Logger(std::shared_ptr<LogSink> sink)
	: _logSink(sink)
{}

Logger::~Logger()
{}

LogMessage Logger::operator()(
	const std::string& file, 
	unsigned int line)
{
	return LogMessage(
		this, file,  line);
}

void Logger::Flush(const LogMessage& message)
{
	if(_logSink)
		_logSink->WriteMessage(message);
}

void Logger::SetLogSink(std::shared_ptr<LogSink> sink)
{
	_logSink = sink;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

