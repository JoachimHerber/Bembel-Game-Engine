#ifndef BEMBEL_BASE_LOGGING_LOGGER_H_
#define BEMBEL_BASE_LOGGING_LOGGER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "log-message.h"

#include <vector>
#include <memory>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class LogSink;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Logger
{
public:
	Logger(std::shared_ptr<LogSink>);
	~Logger();
	
	LogMessage operator()(
		const std::string& file,
		unsigned int line);
	
	void Flush(const LogMessage& message);

	void SetLogSink(std::shared_ptr<LogSink>);

	static Logger info;
	static Logger error;
	static Logger debug;
	static Logger warning;

private:
	std::shared_ptr<LogSink> _logSink;
};

#define BEMBEL_LOG_INFO()    bembel::Logger::info(__FILE__,__LINE__)
#define BEMBEL_LOG_ERROR()   bembel::Logger::error(__FILE__,__LINE__)
#define BEMBEL_LOG_DEBUG()   bembel::Logger::debug(__FILE__,__LINE__)
#define BEMBEL_LOG_WARNING() bembel::Logger::warning(__FILE__,__LINE__)

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
