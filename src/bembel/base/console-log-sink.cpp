module;
#pragma warning(disable : 4005) // macro redefinition
#pragma warning(disable : 5106) // macro redefinition with different parameter names

#include <windows.h>

#include <iostream>
#include <sstream>
#include <stacktrace>
#include <string>
module bembel.base;

import :Logger;
import :ConsoleLogSink;

namespace bembel::base {
ConsoleLogSink::ConsoleLogSink(In<std::string_view> prefix, In<int> color, In<bool> log_call_stack)
  : m_prefix(prefix), m_color(color), m_log_call_stack{log_call_stack} {
    ::SetConsoleOutputCP(CP_UTF8);
}

void ConsoleLogSink::writeMessage(In<std::string_view> message, In<uint> indentation) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_color);

    std::string indent;
    indent.resize(indentation * 2LL, ' ');
    std::string_view remaining = message;
    for(bool first_line = true; !remaining.empty(); first_line = false) {
        auto             line_brack = remaining.find('\n');
        std::string_view line       = remaining.substr(0, line_brack);
        std::cout.write(indent.data(), indent.size());
        if(first_line) { std::cout.write(m_prefix.data(), m_prefix.size()); }
        std::cout.write(line.data(), line.size());
        std::cout.write("\n", 1);
        if(line_brack == std::string_view::npos) break;

        remaining = remaining.substr(line_brack + 1);
    }

    if(m_log_call_stack) {
        auto stacktrace = std::stacktrace::current();
        for(const auto& entry : stacktrace) {
            std::filesystem::path path = entry.source_file();
            auto                  file = path.filename().string();
            auto                  line = entry.source_line();
            if(file == "console-log-sink.cpp" || file == "logger.ixx" || file.empty()
               || line == 0xF00'F00 // Used by VisualStudio for code generteted for coroutine
            ) {
                continue;
            }
            std::cout << std::format("{:>24}(Line:{:>3}): {}\n", file, line, entry.description());
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
}

//*
Logger Logger::info{std::make_shared<ConsoleLogSink>("", 0x0F)};
Logger Logger::error{std::make_shared<ConsoleLogSink>("###ERROR### ", 0xCE, true)};
Logger Logger::debug{std::make_shared<ConsoleLogSink>("", 0x09)};
Logger Logger::warning{std::make_shared<ConsoleLogSink>("- WARNING - ", 0xE0)};
/*/
Logger Logger::info{};
Logger Logger::error{};
Logger Logger::debug{};
Logger Logger::warning{};
//*/
} // namespace bembel::base
