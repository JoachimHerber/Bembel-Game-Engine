module;
#include "bembel/pch.h"
export module bembel.physics:Units;

import bembel.base;

export namespace bembel::physics::units {

template <int _T, int _L, int _M> struct Quantity {
    static inline constexpr int T = _T; // Time
    static inline constexpr int L = _L; // Length
    static inline constexpr int M = _M; // Mass
};

// clang-format off
template <typename T>                 constexpr bool IS_QUANTITY                    = false;
template <int T, int L, int M>        constexpr bool IS_QUANTITY<Quantity<T, L, M>> = true;

template <typename T>                 constexpr bool IS_RATIO                       = false;
template <intmax_t Num, intmax_t Den> constexpr bool IS_RATIO<std::ratio<Num, Den>> = true;

template <typename T> concept QuantityType = IS_QUANTITY<T>;
template <typename T> concept RatioType    = IS_RATIO<T>;
// clang-format on

template <QuantityType T0, QuantityType T1>
using Product = Quantity<
    T0::T + T1::T,
    T0::L + T1::L,
    T0::M + T1::M>;
template <QuantityType T0, QuantityType T1>
using Quotient = Quantity<
    T0::T - T1::T,
    T0::L - T1::L,
    T0::M - T1::M>;

using Scalar = Quantity<0, 0, 0>;

using Time              = Quantity<1, 0, 0>;
using Length            = Quantity<0, 1, 0>;
using Mass              = Quantity<0, 0, 1>;

using Area         = Product<Length, Length>;
using Volume       = Product<Area, Length>;
using Velocity     = Quotient<Length, Time>;
using Acceleration = Quotient<Velocity, Time>;
using Density      = Quotient<Mass, Volume>;

using Frequency = Quotient<Scalar, Time>;
using Force     = Product<Mass, Acceleration>;
using Pressure  = Quotient<Force, Area>;
using Energy    = Product<Force, Length>;
using Power     = Quotient<Energy, Time>;

template <typename TRep, QuantityType TQuantity, RatioType TRatio = std::ratio<1>> struct Unit {
    using Type     = TRep;
    using Quantity = TQuantity;
    using Ratio    = TRatio;
    TRep value;
};

template <typename T, QuantityType Q, RatioType R>
Unit<T, Q, R> operator+(Unit<T, Q, R> a, Unit<T, Q, R> b) {
    return {a.value + b.value};
}

template <typename T, QuantityType Q, RatioType R>
Unit<T, Q, R> operator-(Unit<T, Q, R> a, Unit<T, Q, R> b) {
    return {a.value + b.value};
}

template <typename T, QuantityType Q0, QuantityType Q1, RatioType R0, RatioType R1>
Unit<T, Product<Q0, Q1>, std::ratio_multiply<R0, R1>> operator*(
    Unit<T, Q0, R0> a, Unit<T, Q1, R1> b
) {
    return {a.value * b.value};
}

template <typename T, QuantityType Q0, QuantityType Q1, RatioType R0, RatioType R1>
Unit<T, Quotient<Q0, Q1>, std::ratio_divide<R0, R1>> operator/(
    Unit<T, Q0, R0> a, Unit<T, Q1, R1> b
) {
    return {a.value / b.value};
}

// clang-format off
template <typename T>                              constexpr bool IS_UNIT                = false;
template <typename T, QuantityType Q, RatioType R> constexpr bool IS_UNIT<Unit<T, Q, R>> = true;

template <typename T> concept UnitType = IS_UNIT<T>;

template <UnitType ToUnit, typename FromType, RatioType FromRatio>
ToUnit unit_cast(Unit<FromType, typename ToUnit::Quantity, FromRatio> a) {
    using Ratio = std::ratio_divide<FromRatio, typename ToUnit::Ratio>;
    using TT    = typename ToUnit::Type;
    using CT    = std::common_type_t<TT, FromType, intmax_t>;

    if constexpr(Ratio::den == 1) {
        if constexpr(Ratio::num == 1)
            return {static_cast<TT>(                a.value)};
        else
            return {static_cast<TT>(static_cast<CT>(a.value) * static_cast<CT>(Ratio::num))};
    } else {
        if constexpr(Ratio::num == 1)
            return {static_cast<TT>(static_cast<CT>(a.value)                               / static_cast<CT>(Ratio::den))};
        else
            return {static_cast<TT>(static_cast<CT>(a.value) * static_cast<CT>(Ratio::num) / static_cast<CT>(Ratio::den))};
    }
}
// clang-format on

namespace units {
    // clang-format off
    template <typename T> using Second   = Unit<T, Time>;
    template <typename T> using Meter    = Unit<T, Length>;
    template <typename T> using Kilogram = Unit<T, Mass>;

    template <typename T> using Hertz  = Unit<T, Frequency>;
    template <typename T> using Newton = Unit<T, Force>;
    template <typename T> using Pascal = Unit<T, Pressure>;
    template <typename T> using Joule  = Unit<T, Energy>;
    template <typename T> using Watt   = Unit<T, Power>;

    template <typename T> using SquareMeter = Unit<T, Area>;
    template <typename T> using CubicMetre  = Unit<T, Volume>;

    template <typename T> using MeterPerSecond = Unit<T, Velocity>;
    template <typename T> using MeterPerSecond² = Unit<T, Acceleration>;

    template <typename T> using NanoSecond  = Unit<T, Length, std::nano>;
    template <typename T> using MicroSecond = Unit<T, Length, std::micro>;
    template <typename T> using MilliSecond = Unit<T, Length, std::milli>;
    template <typename T> using Minute      = Unit<T, Length, std::ratio<60>>;
    template <typename T> using Hour        = Unit<T, Length, std::ratio<3600>>;
    template <typename T> using Day         = Unit<T, Length, std::ratio<86400>>;

    template <typename T> using NanoMeter  = Unit<T, Length, std::nano>;
    template <typename T> using MicroMeter = Unit<T, Length, std::micro>;
    template <typename T> using MilliMeter = Unit<T, Length, std::milli>;
    template <typename T> using KiloMeter  = Unit<T, Length, std::kilo>;
    // clang-format on

    namespace literals {
        // clang-format off
        Second     <base::u64> operator""_s (unsigned long long v) { return {v}; }
        Meter      <base::u64> operator""_m (unsigned long long v) { return {v}; }
        SquareMeter<base::u64> operator""_m²(unsigned long long v) { return {v}; }
        CubicMetre <base::u64> operator""_m³(unsigned long long v) { return {v}; }
        Kilogram   <base::u64> operator""_kg(unsigned long long v) { return {v}; }
        
        Second     <double> operator""_s (long double v) { return {double(v)}; }
        Meter      <double> operator""_m (long double v) { return {double(v)}; }
        SquareMeter<double> operator""_m²(long double v) { return {double(v)}; }
        CubicMetre <double> operator""_m³(long double v) { return {double(v)}; }
        Kilogram   <double> operator""_kg(long double v) { return {double(v)}; }

        MeterPerSecond   <base::u64> operator""_mps(unsigned long long v) { return {v}; }
        MeterPerSecond   <double>    operator""_mps(long double        v) { return {double(v)}; }
        // clang-format on
    } // namespace literals

} // namespace units

} // namespace bembel::physics::si