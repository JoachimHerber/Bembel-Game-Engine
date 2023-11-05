module;
#include <string>
#include <string_view>
export module bembel.base:ConsoleLogSink;

import :Logger;

namespace bembel::base {

export class ConsoleLogSink : public Logger::LogSink {
  public:
    ConsoleLogSink(In<std::string_view> prefix, In<int> color, In<bool> log_call_stack = false);
    ~ConsoleLogSink() = default;

    virtual void writeMessage(In<std::string_view> message, In<uint> indentation) override;

  private:
    std::string m_prefix;
    int         m_color;
    bool        m_log_call_stack;
};

} // namespace bembel::base
