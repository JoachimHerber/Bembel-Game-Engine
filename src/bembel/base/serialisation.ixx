module;
#include "bembel/pch.h"
export module bembel.base:Serialisation;

import :Types;
import :ObservableValue;
import :Conversion;
import :Unicode;
import :Xml;

namespace bembel::base {

export namespace serialisation {

    template <typename TClass, typename TMember>
    struct Member {
        using MemberPtr = TMember TClass::*;

        std::string_view name;
        MemberPtr        ptr;

        constexpr Member(MemberPtr p, std::string_view n = "") : name{n}, ptr{p} {}
    };

    template <typename TClass, typename... TMember>
    struct MetaData {
        std::tuple<Member<TClass, TMember>...> member;

        constexpr MetaData(Member<TClass, TMember>... m) : member{m...} {}
    };

    template <typename TClass>
    struct TypeTraits {};

    template <typename TClass>
    requires requires() {
        TClass::SERIALISATION_METADATA;
    }
    struct TypeTraits<TClass> {
        static inline constexpr auto METADATA = TClass::SERIALISATION_METADATA;
    };

    // template <>
    // struct TypeTraits<vec3> {
    //     static inline constexpr auto METADATA = MetaData{
    //          Member{"x", &vec3::x},
    //          Member{"y", &vec3::y},
    //          Member{"z", &vec3::z}
    //     };
    // };
} // namespace serialisation

} // namespace bembel::base