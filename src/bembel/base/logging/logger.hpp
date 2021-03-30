#ifndef BEMBEL_BASE_LOGGING_LOGGER_HPP
#define BEMBEL_BASE_LOGGING_LOGGER_HPP

#include "./log-message.hpp"

namespace bembel::base {

class LogSink;

class BEMBEL_API Logger {
  public:
    Logger(std::shared_ptr<LogSink>);
    ~Logger();

    LogMessage operator()(const std::string& file, unsigned int line);

    void flush(const LogMessage& message);

    void setLogSink(std::shared_ptr<LogSink>);

    void incrementIndentation();
    void decrementIndentation();

    static Logger info;
    static Logger debug;
    static Logger warning;
    static Logger error;

  private:
    std::shared_ptr<LogSink> log_sink;
    unsigned int indentation;
};

class IndentDefaultLogs {
  public:
    IndentDefaultLogs() {
        Logger::info.incrementIndentation();
        Logger::debug.incrementIndentation();
        Logger::warning.incrementIndentation();
        Logger::error.incrementIndentation();
    }
    ~IndentDefaultLogs() {
        Logger::info.decrementIndentation();
        Logger::debug.decrementIndentation();
        Logger::warning.decrementIndentation();
        Logger::error.decrementIndentation();
    }
};

#define BEMBEL_LOG_INFO() bembel::base::Logger::info(__FILE__, __LINE__)
#define BEMBEL_LOG_DEBUG() bembel::base::Logger::debug(__FILE__, __LINE__)
#define BEMBEL_LOG_WARNING() bembel::base::Logger::warning(__FILE__, __LINE__)
#define BEMBEL_LOG_ERROR() bembel::base::Logger::error(__FILE__, __LINE__)

} // namespace bembel::base
#endif // include guards
