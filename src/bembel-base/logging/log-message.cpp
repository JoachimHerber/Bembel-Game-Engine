/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "log-message.h"
#include "logger.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

LogMessage::LogMessage(
		Logger* logger, 
		const std::string& file, 
		unsigned int line)
	: _logger(logger)
	, _file(file)
	, _line(line)
{}

LogMessage::LogMessage(LogMessage&& other)
	: _logger(std::move(other._logger))
	, _file(std::move(other._file))
	, _function(std::move(other._function))
	, _line(std::move(other._line))
	, _buffer(std::move(other._buffer))
{
	other._logger = nullptr;
}

LogMessage::~LogMessage()
{
	_logger->Flush(*this);
}

const std::string& LogMessage::GetFile() const
{
	return _file;
}

unsigned int LogMessage::GetLine() const
{
	return _line;
}

std::string LogMessage::GetDescription() const
{
	return _buffer.str();
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
