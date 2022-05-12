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
#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)

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

#elif defined(__WINDOWS__)

#	include <sys/param.h>

#	define __BYTE_ORDER    BYTE_ORDER
#	define __BIG_ENDIAN    BIG_ENDIAN
#	define __LITTLE_ENDIAN LITTLE_ENDIAN
#	define __PDP_ENDIAN    PDP_ENDIAN

#else
#	error platform not supported
#endif

/* Conversion functions */
#include <type_traits>

// If native byte order is little endian: does nothing
// otherwise: swaps the bytes of x
template<typename integral>
static inline integral little_endian(integral x)
{
    static_assert(std::is_integral<integral>::value, "expected an integral type");
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        return x;
    #elif __BYTE_ORDER == __BIG_ENDIAN

    unsigned char bytes[sizeof(x)];
    for(int i=0; i<sizeof(x); x>>=8)
        bytes[i++] = (unsigned char)(x & 0xff);
    return *reinterpret_cast<integral*>(bytes);

    #else 
    #   error unsupported endianness
    #endif
}

// If native byte order is big endian: does nothing
// otherwise: swaps the bytes of x
template<typename integral>
static inline integral big_endian(integral x){
    static_assert(std::is_integral<integral>::value, "expected an integral type");
    #if __BYTE_ORDER == __BIG_ENDIAN
        return x;
    #elif __BYTE_ORDER == __LITTLE_ENDIAN

    unsigned char bytes[sizeof(x)];
    for(int i=sizeof(x)-1; i>=0; x>>=8)
        bytes[i--] = (unsigned char)(x & 0xff);
    return *reinterpret_cast<integral*>(bytes);
    
    #else
    #   error unsupported endianness
    #endif
}