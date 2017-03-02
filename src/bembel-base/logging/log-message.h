#ifndef BEMBEL_BASE_LOGGING_LOGMESSAGE_H_
#define BEMBEL_BASE_LOGGING_LOGMESSAGE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <sstream>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class Logger;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API LogMessage
{
	LogMessage(Logger*, const std::string&, unsigned int);
public:
	LogMessage(LogMessage&&);
	~LogMessage();

	template <typename T>
	LogMessage& operator<< (const T& value);
	LogMessage& operator<< (std::ostream& (*fn)(std::ostream&));

	const std::string& GetFile() const;
	unsigned int       GetLine() const;

	std::string GetDescription() const;

private:
	Logger* _logger;

	std::string  _file;
	std::string  _function;
	unsigned int _line;

	std::stringstream _buffer;

	friend class Logger;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/
namespace bembel {

template <typename T>
inline LogMessage& LogMessage::operator<<(const T& value)
{
	_buffer << value;
	return *this;
}
inline LogMessage& LogMessage::operator<<(std::ostream& (*fn)(std::ostream&))
{
	_buffer << fn;
	return *this;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
