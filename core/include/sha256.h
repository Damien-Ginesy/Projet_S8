/*
* The MIT License (MIT)
* Copyright © 2021 EddieBreeg
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files (the “Software”), to deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
* permit persons to whom the Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
* Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#if defined(__cplusplus)
extern "C"{
#endif // __cplusplus


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

typedef uint8_t byte;

#define ROTR(x,n) ((x>>n) | (x<<(32-n)))

#define FROM_BIG_ENDIAN(b) (((b)[0]<<24) | ((b)[1]<<16) | ((b)[2]<<8) | (b)[3])

// computes the hash of the data buffer
void sha256_hash(const void* data, size_t len, byte output[32]);
// computers the hash of whatever file is passed
void sha256_hash_file(FILE* file, byte output[32]);

#if defined(__cplusplus)
}
#endif // __cplusplus
