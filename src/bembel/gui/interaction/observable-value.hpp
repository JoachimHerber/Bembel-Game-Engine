#ifndef BEMBEL_GUI_INTERACTION_OBSERVABLEVALUE_HPP
#define BEMBEL_GUI_INTERACTION_OBSERVABLEVALUE_HPP

#include <bembel/base/utils/signal.hpp>

namespace bembel::gui {

template <typename ValueType>
class ObservableValue final {
  public:
    ObservableValue();
    ObservableValue(const ValueType& value);
    ~ObservableValue();

    /**
     * ChangeSignals are used to signalize changes to an ObservableValue.
     * Signale arguments:
     * 1. const ValueType& old_value;
     * 2. const ValueType& new_value;
     */
    using ChangeSignal = base::Signal<const ValueType&, const ValueType&>;

    /**
     * ConstrainSignal can be used to constrain changes to an ObservableValue.
     * Signale arguments:
     * 1. const ValueType& old_value;
     * 2. ValueType& new_value;
     */
    using ConstrainSignal = base ::Signal<const ValueType&, ValueType&>;

    ChangeSignal change_signal;
    ConstrainSignal constrain_signal;

    const ValueType& get() const;
    bool set(ValueType&& value);
    bool set(const ValueType& value);

    ObservableValue<ValueType>& operator=(const ObservableValue<ValueType>& other);

  private:
    ValueType value;
};

} // namespace bembel::gui

// inline implementation
#include "observable-value.inl"
#endif // include guards
