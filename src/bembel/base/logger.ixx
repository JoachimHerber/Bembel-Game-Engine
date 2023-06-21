module;
#include <format>
#include <memory>
#include <source_location>
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

        virtual void writeMessage(
            In<std::source_location> location, In<std::string_view> message, In<u32> indentation
        ) = 0;
    };

  public:
    Logger() = default;
    Logger(Move<std::shared_ptr<LogSink>> sink) : m_log_sink{sink} {}
    Logger(In<std::shared_ptr<LogSink>> sink) : m_log_sink{sink} {}
    ~Logger() = default;

    void log(In<std::source_location> location, In<std::string_view> message) {
        if(m_log_sink) m_log_sink->writeMessage(location, message, m_indentation);
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

export class LogMassage {
  public:
    LogMassage(std::source_location location) : m_location(location) {}
    LogMassage(LogMassage&& other)      = default;
    LogMassage(LogMassage const& other) = default;
    ~LogMassage()                       = default;

    void info(In<std::string_view> message) { Logger::info.log(m_location, message); }
    void error(In<std::string_view> message) { Logger::error.log(m_location, message); }
    void debug(In<std::string_view> message) { Logger::debug.log(m_location, message); }
    void warning(In<std::string_view> message) { Logger::warning.log(m_location, message); }

    template <typename... TArgs>
    void info(In<std::string_view> format, TArgs&&... args) {
        info(std::vformat(format, std::make_format_args(std::forward<TArgs>(args)...)));
    }
    template <typename... TArgs>
    void error(In<std::string_view> format, TArgs&&... args) {
        error(std::vformat(format, std::make_format_args(std::forward<TArgs>(args)...)));
    }
    template <typename... TArgs>
    void debug(In<std::string_view> format, TArgs&&... args) {
        debug(std::vformat(format, std::make_format_args(std::forward<TArgs>(args)...)));
    }
    template <typename... TArgs>
    void warning(In<std::string_view> format, TArgs&&... args) {
        warning(std::vformat(format, std::make_format_args(std::forward<TArgs>(args)...)));
    }

  private:
    std::source_location m_location;
};

export inline LogMassage log(std::source_location location = std::source_location::current()) {
    return {location};
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
