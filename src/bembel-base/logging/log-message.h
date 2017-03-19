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
	Logger* logger_;

	std::string  file_;
	std::string  function_;
	unsigned int line_;

	std::stringstream buffer_;

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
	buffer_ << value;
	return *this;
}
inline LogMessage& LogMessage::operator<<(std::ostream& (*function)(std::ostream&))
{
	buffer_ << function;
	return *this;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
