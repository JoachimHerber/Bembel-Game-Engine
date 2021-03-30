#include "./log-message.hpp"

#include "./logger.hpp"

namespace bembel::base {

LogMessage::LogMessage(Logger* logger, const std::string& file, unsigned int line)
: logger(logger)
, file(file)
, line(line) {
}

LogMessage::LogMessage(LogMessage&& other)
: logger(std::move(other.logger))
, file(std::move(other.file))
, function(std::move(other.function))
, line(std::move(other.line))
, buffer(std::move(other.buffer)) {
    other.logger = nullptr;
}

LogMessage::~LogMessage() {
    this->logger->flush(*this);
}

const std::string& LogMessage::getFile() const {
    return this->file;
}

unsigned int LogMessage::getLine() const {
    return this->line;
}

std::string LogMessage::getDescription() const {
    return this->buffer.str();
}

} // namespace bembel::base
