module;
#include <windows.h>

#include <iostream>
#include <sstream>
#include <string>

module bembel.base;

import :Logger;

namespace bembel::base {
ConsoleLogSink::ConsoleLogSink(In<std::string_view> prefix, In<int> color)
  : m_prefix(prefix)
  , m_color(color) {
    ::SetConsoleOutputCP(CP_UTF8);
}

void ConsoleLogSink::writeMessage(
    In<std::source_location>, In<std::string_view> message, In<uint> indentation
) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_color);

    std::string prefix;
    prefix.resize(indentation * 2LL, ' ');

    for(bool first_line = true; !message.empty();) {
        auto             line_brack = message.find('\n');
        std::string_view line       = message.substr(0, line_brack);
        if(first_line) {
            std::cout << prefix << m_prefix << line << std::endl;
            first_line = false;
        } else {
            std::cout << prefix << line << std::endl;
        }
        if(line_brack == std::string_view::npos) break;

        message = message.substr(line_brack);
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
}

//*
Logger Logger::info {std::make_shared<ConsoleLogSink>("", 0x0F)};
Logger Logger::error{std::make_shared<ConsoleLogSink>("###ERROR### ", 0xCE)};
Logger Logger::debug{std::make_shared<ConsoleLogSink>("", 0x09)};
Logger Logger::warning{std::make_shared<ConsoleLogSink>("- WARNING - ", 0xE0)};
/*/
Logger Logger::info{};
Logger Logger::error{};
Logger Logger::debug{};
Logger Logger::warning{};
//*/
} // namespace bembel::base
