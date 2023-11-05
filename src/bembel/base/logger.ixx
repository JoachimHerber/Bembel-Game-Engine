module;
#include <format>
#include <memory>
#include <string>
#include <string_view>
export module bembel.base:Logger;

import :Types;

namespace bembel::base {

export class Logger {
  public:
    class LogSink {
      public:
        LogSink()          = default;
        virtual ~LogSink() = default;

        virtual void writeMessage(In<std::string_view> message, In<u32> indentation) = 0;
    };

  public:
    Logger() = default;
    Logger(Move<std::shared_ptr<LogSink>> sink) : m_log_sink{sink} {}
    Logger(In<std::shared_ptr<LogSink>> sink) : m_log_sink{sink} {}
    ~Logger() = default;

    void log(In<std::string_view> message) {
        if(m_log_sink) m_log_sink->writeMessage(message, m_indentation);
    }

    void setLogSink(std::shared_ptr<LogSink> const& sink) { m_log_sink = sink; }
    void setLogSink(std::shared_ptr<LogSink>&& sink) { m_log_sink = sink; }

    void incrementIndentation() { m_indentation++; }
    void decrementIndentation() {
        if(m_indentation > 0) m_indentation--;
    }

    static Logger info;
    static Logger debug;
    static Logger warning;
    static Logger error;

  private:
    std::shared_ptr<LogSink> m_log_sink    = nullptr;
    u32                      m_indentation = 0;
};

export void logInfo(In<std::string_view> message) {
    Logger::info.log(message);
}
export void logError(In<std::string_view> message) {
    Logger::error.log(message);
}
export void logDebug(In<std::string_view> message) {
    Logger::debug.log(message);
}
export void logWarning(In<std::string_view> message) {
    Logger::warning.log(message);
}

export template <typename... TArgs>
void logInfo(In<std::string_view> format, TArgs&&... args) {
    logInfo(std::vformat(format, std::make_format_args(std::forward<TArgs>(args)...)));
}
export template <typename... TArgs>
void logError(In<std::string_view> format, TArgs&&... args) {
    logError(std::vformat(format, std::make_format_args(std::forward<TArgs>(args)...)));
}
export template <typename... TArgs>
void logDebug(In<std::string_view> format, TArgs&&... args) {
    logDebug(std::vformat(format, std::make_format_args(std::forward<TArgs>(args)...)));
}
export template <typename... TArgs>
void logWarning(In<std::string_view> format, TArgs&&... args) {
    logWarning(std::vformat(format, std::make_format_args(std::forward<TArgs>(args)...)));
}

export class IndentDefaultLogs {
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

} // namespace bembel::base
