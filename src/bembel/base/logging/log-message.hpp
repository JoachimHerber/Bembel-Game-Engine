#ifndef BEMBEL_BASE_LOGGING_LOGMESSAGE_HPP
#define BEMBEL_BASE_LOGGING_LOGMESSAGE_HPP

#include <bembel/library.hpp>

namespace bembel::base {

class Logger;

class BEMBEL_API LogMessage {
    LogMessage(Logger*, const std::string&, unsigned int);

  public:
    LogMessage(LogMessage&&);
    ~LogMessage();

    template <typename T>
    LogMessage& operator<<(const T& value);
    LogMessage& operator<<(std::ostream& (*fn)(std::ostream&));

    const std::string& getFile() const;
    unsigned int getLine() const;

    std::string getDescription() const;

  private:
    Logger* logger;

    std::string file;
    std::string function;
    unsigned int line;

    std::stringstream buffer;

    friend class Logger;
};

template <typename T>
inline LogMessage& LogMessage::operator<<(const T& value) {
    this->buffer << value;
    return *this;
}

inline LogMessage& LogMessage::operator<<(std::ostream& (*function)(std::ostream&)) {
    this->buffer << function;
    return *this;
}

} // namespace bembel::base
#endif // include guards
