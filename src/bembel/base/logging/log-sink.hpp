#ifndef BEMBEL_BASE_LOGGING_LOGSINK_HPP
#define BEMBEL_BASE_LOGGING_LOGSINK_HPP

#include <bembel/library.hpp>

namespace bembel::base {

class LogMessage;

class BEMBEL_API LogSink {
  public:
    LogSink()          = default;
    virtual ~LogSink() = default;

    virtual void writeMessage(const LogMessage&, unsigned int indentation) = 0;
};

} // namespace bembel::base
#endif // include guards
