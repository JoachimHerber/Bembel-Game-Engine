#include "./console-log-sink.hpp"

#include "./log-message.hpp"

#ifdef _WIN32
#    include <windows.h>
#endif

namespace bembel::base {

ConsoleLogSink::ConsoleLogSink(const std::string& prefix, int color)
: prefix(prefix)
, color(color) {
}

ConsoleLogSink::~ConsoleLogSink() {
}

void ConsoleLogSink::writeMessage(const LogMessage& msg, unsigned int indentation) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), this->color);
#endif

    std::string indent_prefix;
    indent_prefix.resize(indentation * 2LL, ' ');

    std::istringstream description{msg.getDescription()};
    std::string line;
    while(std::getline(description, line)) {
        std::cout << indent_prefix << this->prefix << line << std::endl;
    }

#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
#endif
}

} // namespace bembel::base
