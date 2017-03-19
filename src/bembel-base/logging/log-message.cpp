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
	: logger_(logger)
	, file_(file)
	, line_(line)
{}

LogMessage::LogMessage(LogMessage&& other)
	: logger_(std::move(other.logger_))
	, file_(std::move(other.file_))
	, function_(std::move(other.function_))
	, line_(std::move(other.line_))
	, buffer_(std::move(other.buffer_))
{
	other.logger_ = nullptr;
}

LogMessage::~LogMessage()
{
	logger_->Flush(*this);
}

const std::string& LogMessage::GetFile() const
{
	return file_;
}

unsigned int LogMessage::GetLine() const
{
	return line_;
}

std::string LogMessage::GetDescription() const
{
	return buffer_.str();
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
