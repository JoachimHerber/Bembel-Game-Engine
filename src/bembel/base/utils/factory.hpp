#ifndef BEMBEL_BASE_UTILS_FACTORY_HPP
#define BEMBEL_BASE_UTILS_FACTORY_HPP

#include <bembel/library.hpp>

namespace bembel::base {

template <typename ObjectType, typename... TArgs>
class Factory {
  public:
    Factory();
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;
    ~Factory();

    using ObjectGenerator = std::function<std::unique_ptr<ObjectType>(TArgs...)>;

    void registerObjectGenerator(const std::string& type, ObjectGenerator object_generator);
    template <typename T>
    void registerDefaultObjectGenerator(const std::string& type);

    std::unique_ptr<ObjectType> createObject(const std::string& type, TArgs... args) const;

  private:
    std::map<std::string, ObjectGenerator> generators;
};

} // namespace bembel::base
#include "Factory.inl"
#endif // include guards
