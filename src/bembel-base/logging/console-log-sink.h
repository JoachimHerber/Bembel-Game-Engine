#ifndef BEMBEL_CONSOLELOGSINK_H
#define BEMBEL_CONSOLELOGSINK_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include "LogSink.h"

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
