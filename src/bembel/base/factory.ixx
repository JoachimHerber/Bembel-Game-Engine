module;
#include <functional>
#include <memory>
#include <string_view>
#include <vector>
export module bembel.base:Factory;

import :Types;
import :Unicode;

export namespace bembel::base {

template <typename ObjectType, typename... TArgs>
class Factory {
  public:
    Factory()  = default;
    ~Factory() = default;

    Factory(Factory&&)                 = delete;
    Factory(Factory const&)            = delete;
    Factory& operator=(Factory&&)      = delete;
    Factory& operator=(Factory const&) = delete;

    using ObjectPtr       = std::unique_ptr<ObjectType>;
    using ObjectGenerator = std::function<ObjectPtr(TArgs...)>;

    bool registerCustomObjectGenerator(
        In<std::string_view> generator_name, In<ObjectGenerator> generator
    ) {
        if(m_generators.contains(generator_name)) return false;

        m_generators.emplace(generator_name, generator);
        return true;
    }

    template <typename T>
    bool registerObjectGenerator(In<std::string_view> generator_name) {
        if(m_generators.contains(generator_name)) return false;

        m_generators.emplace(generator_name, [](TArgs... args) {
            return std::make_unique<T>(args...);
        });
        return true;
    }

    template <typename T>
        requires requires() { { T::getTypeName() } -> std::convertible_to<std::string_view>; }
    bool registerObjectGenerator() {
        return registerObjectGenerator<T>(T::getTypeName());
    }

    ObjectPtr createObject(In<std::string_view> generator_name, TArgs... args) const {
        auto it = m_generators.find(generator_name);

        if(it == m_generators.end()) return {};

        return it->second(args...);
    }

  private:
    Dictionary<ObjectGenerator> m_generators;
};

} // namespace bembel::base
