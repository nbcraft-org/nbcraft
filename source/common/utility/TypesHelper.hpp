#pragma once

#include <stdint.h>

template<typename T>
struct IsIntegral
{
    enum { value = false };
};

template<> struct IsIntegral<bool> { enum { value = true }; };
template<> struct IsIntegral<int8_t> { enum { value = true }; };
template<> struct IsIntegral<uint8_t> { enum { value = true }; };
template<> struct IsIntegral<int16_t> { enum { value = true }; };
template<> struct IsIntegral<uint16_t> { enum { value = true }; };
template<> struct IsIntegral<int32_t> { enum { value = true }; };
template<> struct IsIntegral<uint32_t> { enum { value = true }; };
template<> struct IsIntegral<int64_t> { enum { value = true }; };
template<> struct IsIntegral<uint64_t> { enum { value = true }; };

template<typename T>
struct IsPointer
{
    enum { value = false };
};

template<typename T>
struct IsPointer<T*>
{
    enum { value = true };
};

template<typename T>
struct IsEnum
{
private:
    static int  check(int);
    static char check(...);
    static T    make();

public:
    enum
    {
        value = sizeof(check(make())) == sizeof(int)
        && !IsIntegral<T>::value
        && !IsPointer<T>::value
    };
};
