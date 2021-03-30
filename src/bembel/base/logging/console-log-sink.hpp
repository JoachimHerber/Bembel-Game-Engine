#ifndef BEMBEL_BASE_LOGGING_CONSOLELOGSINK_HPP
#define BEMBEL_BASE_LOGGING_CONSOLELOGSINK_HPP

#include "./log-sink.hpp"

namespace bembel::base {

class BEMBEL_API ConsoleLogSink : public LogSink {
  public:
    ConsoleLogSink(const std::string&, int);
    ~ConsoleLogSink();

    virtual void writeMessage(const LogMessage&, unsigned int indentation) override;

  private:
    std::string prefix;
    int color;
};

} // namespace bembel::base
#endif // include guards
