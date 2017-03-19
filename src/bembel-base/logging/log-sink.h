#ifndef BEMBEL_BASE_LOGGING_LOGSINK_H_
#define BEMBEL_BASE_LOGGING_LOGSINK_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class LogMessage;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API LogSink
{
public:
	LogSink();
	virtual ~LogSink();

	virtual void WriteMessage(const LogMessage&) = 0;

private:
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards

