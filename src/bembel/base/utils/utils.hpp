#ifndef BEMBEL_BASE_UTILS_HPP
#define BEMBEL_BASE_UTILS_HPP

#include <bembel/library.hpp>

#define BEMBEL_STATIC_INITIALIZATION                                                               \
    void performInitialization();                                                                  \
    static bool dummy = (performInitialization(), true);                                           \
    void performInitialization()

namespace bembel::base {

template <typename T>
class reverse {
  private:
    T& iterable_;

  public:
    explicit reverse(T& iterable)
    : iterable_{iterable} {}
    auto begin() const { return std::rbegin(iterable_); }
    auto end() const { return std::rend(iterable_); }
};

} // namespace bembel::base

#endif // include guards
