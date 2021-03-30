#include "./logger.hpp"

#include "./console-log-sink.hpp"
#include "./log-sink.hpp"

namespace bembel::base {

Logger Logger::info(std::make_shared<ConsoleLogSink>("", 0x0F));
Logger Logger::error(std::make_shared<ConsoleLogSink>("###ERROR###\t", 0xCE));
Logger Logger::debug(std::make_shared<ConsoleLogSink>("", 0x09));
Logger Logger::warning(std::make_shared<ConsoleLogSink>("- WARNING -\t", 0xE0));

Logger::Logger(std::shared_ptr<LogSink> sink)
: log_sink(sink) {
}

Logger::~Logger() {
}

LogMessage Logger::operator()(const std::string& file, unsigned int line) {
    return LogMessage(this, file, line);
}

void Logger::flush(const LogMessage& message) {
    if(this->log_sink) this->log_sink->writeMessage(message, this->indentation);
}

void Logger::setLogSink(std::shared_ptr<LogSink> sink) {
    this->log_sink = sink;
}

void Logger::incrementIndentation() {
    this->indentation++;
}

void Logger::decrementIndentation() {
    if(this->indentation > 0) this->indentation--;
}

} // namespace bembel::base
