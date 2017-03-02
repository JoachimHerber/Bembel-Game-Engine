#ifndef BEMBEL_BASE_LOGGING_CONSOLELOGSINK_H_
#define BEMBEL_BASE_LOGGING_CONSOLELOGSINK_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "log-sink.h"

#include <string>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API ConsoleLogSink : public LogSink
{
public:
	ConsoleLogSink(const std::string&, int);
	~ConsoleLogSink();

	virtual void WriteMessage(const LogMessage&) override;

private:
	std::string _prefix;
	int         _color;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
