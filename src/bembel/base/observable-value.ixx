module;
#include <utility>
export module bembel.base:ObservableValue;

import :Signal;
import :Types;

namespace bembel::base {

export template <typename T>
class ObservableValue final {
  public:
    ObservableValue() = default;
    ObservableValue(In<T> value) : m_value(value) {}
    template <typename... TArgs>
    ObservableValue(TArgs&&... args) : m_value(std::forward<TArgs>(args)...) {}
    ~ObservableValue() = default;

    using Type = T;
    /**
     * ChangeSignals are used to signalize changes to an ObservableValue.
     * Signale arguments:
     * 1. In<T> old_value;
     * 2. In<T> new_value;
     */
    using ChangeSignal = base::Signal<In<T>, In<T>>;

    /**
     * ConstrainSignal can be used to constrain changes to an ObservableValue.
     * Signale arguments:
     * 1. In<T>    old_value;
     * 2. InOut<T> new_value;
     */
    using ConstrainSignal = base ::Signal<In<T>, InOut<T>>;

    ChangeSignal    change_signal;
    ConstrainSignal constrain_signal;

    T const& get() const { return m_value; }

    bool set(T new_value) {
        if(new_value == m_value) return false;

        this->constrain_signal.emit(m_value, new_value);
        if(new_value == m_value) return false;

        T old_value = std::move(m_value);
        m_value     = std::move(new_value);

        this->change_signal.emit(old_value, m_value);
        return true;
    }

    ObservableValue<T>& operator=(ObservableValue<T> const& other) {
        set(other.get());
        return *this;
    }

    ObservableValue<T>& operator=(T value) {
        set(std::move(value));
        return *this;
    }

    operator T const&() const { return m_value; }

  private:
    T m_value;
};

template <typename T>
struct IsObservableValue : std::false_type {};
template <typename T>
struct IsObservableValue<ObservableValue<T>> : std::true_type {};
} // namespace bembel::base
