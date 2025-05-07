#pragma once

#include <base/Logger.hpp>

namespace neo {

    typedef int8_t    i8;
    typedef uint8_t   u8;
    typedef int16_t   i16;
    typedef uint16_t  u16;
    typedef int32_t   i32;
    typedef uint32_t  u32;
    typedef int64_t   i64;
    typedef uint64_t  u64;
    typedef float     f32;
    typedef double    f64;

    typedef int_fast32_t  if32;
    typedef uint_fast32_t uf32;

    typedef size_t  ptr_size;
    typedef u8      byte;
    typedef char    sbyte;
    typedef wchar_t wchar;

    constexpr i8 kInt8Max = std::numeric_limits<i8>::max();
    constexpr i8 kInt8Min = std::numeric_limits<i8>::min();

    constexpr i16 kInt16Max = std::numeric_limits<i16>::max();
    constexpr i16 kInt16Min = std::numeric_limits<i16>::min();

    constexpr i32 kInt32Max = std::numeric_limits<i32>::max();
    constexpr i32 kInt32Min = std::numeric_limits<i32>::min();

    constexpr i64 kInt64Max = std::numeric_limits<i64>::max();
    constexpr i64 kInt64Min = std::numeric_limits<i64>::min();

    constexpr if32 kFastIntMax = std::numeric_limits<if32>::max();
    constexpr if32 kFastIntMin = std::numeric_limits<if32>::min();

    constexpr u8 kUInt8Max = std::numeric_limits<u8>::max();
    constexpr u8 kUInt8Min = std::numeric_limits<u8>::min();

    constexpr u16 kUInt16Max = std::numeric_limits<u16>::max();
    constexpr u16 kUInt16Min = std::numeric_limits<u16>::min();

    constexpr u32 kUInt32Max = std::numeric_limits<u32>::max();
    constexpr u32 kUInt32Min = std::numeric_limits<u32>::min();

    constexpr u64 kUInt64Max = std::numeric_limits<u64>::max();
    constexpr u64 kUInt64Min = std::numeric_limits<u64>::min();

    constexpr uf32 kFastUIntMax = std::numeric_limits<uf32>::max();
    constexpr uf32 kFastUIntMin = std::numeric_limits<uf32>::min();

    constexpr ptr_size kMaxPtrSize = std::numeric_limits<ptr_size>::max();
    constexpr ptr_size kMinPtrSize = std::numeric_limits<ptr_size>::min();
    static_assert(sizeof(ptr_size) == sizeof(void*), "ptr_size is not vailed!");

    constexpr f32 kF32Max = std::numeric_limits<f32>::max();
    constexpr f32 kF32Min = -std::numeric_limits<f32>::max();
    constexpr f64 kF64Max = std::numeric_limits<f64>::max();
    constexpr f64 kF64Min = -std::numeric_limits<f64>::max();
    static_assert(sizeof(bool) == 1, "bool is not vailed!");


    template <typename T, u32 count>
    constexpr u32 lengthOf(const T(&)[count])
    {
        return count;
    };
}