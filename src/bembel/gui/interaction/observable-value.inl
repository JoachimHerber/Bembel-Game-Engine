namespace bembel::gui {

template <typename ValueType>
inline ObservableValue<ValueType>::ObservableValue() {
}

template <typename ValueType>
inline ObservableValue<ValueType>::ObservableValue(const ValueType& value)
: value(value) {
}

template <typename ValueType>
inline ObservableValue<ValueType>::~ObservableValue() {
}

template <typename ValueType>
inline const ValueType& ObservableValue<ValueType>::get() const {
    return this->value;
}

template <typename ValueType>
inline bool ObservableValue<ValueType>::set(ValueType&& new_value) {
    if(new_value == this->value) return false;

    this->constrain_signal.emit(this->value, new_value);
    if(new_value == this->value) return false;

    ValueType old_value = std::move(this->value);
    this->value         = std::move(new_value);

    this->change_signal.emit(old_value, this->value);
    return true;
}

template <typename ValueType>
inline bool ObservableValue<ValueType>::set(const ValueType& value) {
    if(value == this->value) return false;

    ValueType new_value = value;
    this->constrain_signal.emit(this->value, new_value);
    if(new_value == this->value) return false;

    ValueType old_value = std::move(this->value);
    this->value         = new_value;

    this->change_signal.emit(old_value, this->value);
    return true;
}

template <typename ValueType>
inline ObservableValue<ValueType>& ObservableValue<ValueType>::operator=(
    const ObservableValue<ValueType>& other) {
    this->set(other.get());
    return *this;
}

} // namespace bembel::gui
