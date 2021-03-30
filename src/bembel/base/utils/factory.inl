namespace bembel::base {

template <typename ObjectType, typename... TArgs>
inline Factory<ObjectType, TArgs...>::Factory() {
}

template <typename ObjectType, typename... TArgs>
inline Factory<ObjectType, TArgs...>::~Factory() {
}

template <typename ObjectType, typename... TArgs>
inline void Factory<ObjectType, TArgs...>::registerObjectGenerator(
    const std::string& type, ObjectGenerator generator) {
    this->generators.emplace(type, generator);
}

template <typename ObjectType, typename... TArgs>
inline std::unique_ptr<ObjectType> Factory<ObjectType, TArgs...>::createObject(
    const std::string& type, TArgs... args) const {
    auto it = this->generators.find(type);

    if(it == this->generators.end()) return nullptr;

    return it->second(args...);
}

template <typename ObjectType, typename... TArgs>
template <typename T>
inline void Factory<ObjectType, TArgs...>::registerDefaultObjectGenerator(const std::string& type) {
    this->generators.emplace(type, [](TArgs... args) { return T::createInstance(args...); });
}

} // namespace bembel::base
