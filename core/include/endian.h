#pragma once

/* 
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.
In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
For more information, please refer to <http://unlicense.org/>
*/

/* Macro definitions (based on original code by Mathias Panzenböck which you can find here:
https://github.com/mikepb/endian.h/blob/master/endian.h) 
*/
#if (defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)

#	define __WINDOWS__

#endif

#if defined(__linux__) || defined(__CYGWIN__)

#	include <endian.h>

#elif defined(__APPLE__)

#	include <libkern/OSByteOrder.h>

#	define __BYTE_ORDER    BYTE_ORDER
#	define __BIG_ENDIAN    BIG_ENDIAN
#	define __LITTLE_ENDIAN LITTLE_ENDIAN
#	define __PDP_ENDIAN    PDP_ENDIAN

#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)

#	include <sys/endian.h>

#elif defined(__WINDOWS__) && !defined(_MSC_VER)

#	include <sys/param.h>

#	define __BYTE_ORDER    BYTE_ORDER
#	define __BIG_ENDIAN    BIG_ENDIAN
#	define __LITTLE_ENDIAN LITTLE_ENDIAN
#	define __PDP_ENDIAN    PDP_ENDIAN
#elif defined(__WINDOWS__) &&\
    (defined(_M_IX86) || defined(_M_X64))

#define     __LITTLE_ENDIAN 0
#define     __BIG_ENDIAN    1
#define     __PDP_ENDIAN    3
#define     __BYTE_ORDER    __LITTLE_ENDIAN

#else
#	error native endianness couldn't be determined'
#endif

#if   (__BYTE_ORDER != __LITTLE_ENDIAN) && ((__BYTE_ORDER != __BIG_ENDIAN)) && (__PDP_ENDIAN == __BYTE_ORDER)
#   error PDP endian not supported 
#endif

#include <stdint.h>

#ifndef __cplusplus

uint16_t little_endian16(uint16_t x){
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    return x;
    #else
    return (x>>8) | (x<<8);
    #endif
}
uint16_t big_endian16(uint16_t x){
    #if __BYTE_ORDER == __BIG_ENDIAN
    return x;
    #else
    return (x>>8) | (x<<8);
    #endif
}
uint32_t little_endian32(uint32_t x){
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    return x;
    #else
    uint8_t b[] = {
        (uint8_t)(x),
        (uint8_t)(x>>8),
        (uint8_t)(x>>16),
        (uint8_t)(x>>24),
    };
    return *(uint32_t*)b;
    #endif
}
uint32_t big_endian32(uint32_t x){
    #if __BYTE_ORDER == __BIG_ENDIAN
    return x;
    #else
    uint8_t b[] = {
        (uint8_t)(x>>24),
        (uint8_t)(x>>16),
        (uint8_t)(x>>8),
        (uint8_t)(x),
    };
    return *(uint32_t*)b;
    #endif
}
uint64_t little_endian64(uint64_t x){
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    return x;
    #else
    uint8_t b[] = {
        (uint8_t)(x),
        (uint8_t)(x>>8),
        (uint8_t)(x>>16),
        (uint8_t)(x>>24),
        (uint8_t)(x>>32),
        (uint8_t)(x>>40),
        (uint8_t)(x>>48),
        (uint8_t)(x>>56),
    };
    return *(uint64_t*)b;
    #endif
}
uint64_t big_endian64(uint64_t x){
    #if __BYTE_ORDER == __BIG_ENDIAN
    return x;
    #else
    uint8_t b[] = {
        (uint8_t)(x>>56),
        (uint8_t)(x>>48),
        (uint8_t)(x>>40),
        (uint8_t)(x>>32),
        (uint8_t)(x>>24),
        (uint8_t)(x>>16),
        (uint8_t)(x>>8),
        (uint8_t)(x),
    };
    return *(uint64_t*)b;
    #endif
}

#else

#include <type_traits>

template<typename integral>
static inline integral little_endian(integral x){
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    return x;
    #else
    static_assert(std::is_integral<integral>::value);
    unsigned char b[sizeof x];
    for(int i=0; x; i++){
        b[i] = (unsigned char)x;
        x >>= 8;
    }
    return *reinterpret_cast<integral*>(b);
    #endif
}
template<typename integral>
static inline integral big_endian(integral x){
    #if __BYTE_ORDER == __BIG_ENDIAN
    return x;
    #else
    static_assert(std::is_integral<integral>::value);
    unsigned char b[sizeof x];
    for(int i=sizeof x-1; i>-1; i--){
        b[i] = (unsigned char)x;
        x >>= 8;
    }
    return *reinterpret_cast<integral*>(b);
    #endif
}

#endif