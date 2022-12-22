module;
#include <string>
#include <string_view>
export module bembel.base:ConsoleLogSink;

import :Logger;
import :Unicode;

namespace bembel::base {

export class ConsoleLogSink : public Logger::LogSink {
  public:
    ConsoleLogSink(In<std::string_view> prefix, In<int> color);
    ~ConsoleLogSink() = default;

    virtual void writeMessage(
        In<std::source_location> location, In<std::string_view> message, In<uint> indentation
    ) override;

  private:
    std::string m_prefix;
    int         m_color;
};

} // namespace bembel::base
