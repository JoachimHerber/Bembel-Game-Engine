#ifndef BEMBEL_LOGSINK_H
#define BEMBEL_LOGSINK_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

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

