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

#include <sha256.h>
#include <stdio.h>


#define Ch(x, y, z) ((x & y) ^ (~x & z))
#define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

#define SIGMA_0(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIGMA_1(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define sigma_0(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ (x >> 3))
#define sigma_1(x) ((ROTR(x, 17) ^ ROTR(x, 19)) ^ (x >> 10))

typedef struct sha256_context
{
    uint32_t block[16];
    uint32_t hash[8];
    size_t dataLength;
} sha256_context;

static uint32_t K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};
static uint32_t pad(byte buffer[128], size_t totalLength)
{
    uint32_t n = totalLength & 63;
    buffer[n] = 0x80;
    int i;
    for(i=n+1; (i & 63) != 56; i++)
        buffer[i] = 0;
    totalLength <<= 3;
    buffer += i;
    for(int j=7; j>=0; j--)
    {
        buffer[j] = (totalLength & 0xff);
        totalLength >>= 8;
    }
    return i<64? 64:128; 
}
void parse(sha256_context* ctx, byte buffer[64])
{
    for(int i=0; i< 16; i++)
        ctx->block[i] = FROM_BIG_ENDIAN(buffer+(i<<2));
}
void init(sha256_context* ctx, size_t length)
{
    // initial internal state
    ctx->hash[0] = 0x6a09e667;
    ctx->hash[1] = 0xbb67ae85;
    ctx->hash[2] = 0x3c6ef372;
    ctx->hash[3] = 0xa54ff53a;
    ctx->hash[4] = 0x510e527f;
    ctx->hash[5] = 0x9b05688c;
    ctx->hash[6] = 0x1f83d9ab;
    ctx->hash[7] = 0x5be0cd19;
    ctx->dataLength = length;
}
void update(sha256_context* ctx)
{
    uint32_t W[64];
    for(int i=0; i<16; i++)
        W[i] = ctx->block[i];
    for(int i=16; i< 64; i++)
        W[i] = sigma_1(W[i-2]) + W[i-7] + sigma_0(W[i-15]) + W[i-16];
    
    uint32_t a,b,c,d,e,f,g,h;

    a = ctx->hash[0];
    b = ctx->hash[1];
    c = ctx->hash[2];
    d = ctx->hash[3];
    e = ctx->hash[4];
    f = ctx->hash[5];
    g = ctx->hash[6];
    h = ctx->hash[7];

    uint32_t T1, T2;

    for (int t = 0; t < 64; t++)
    {
        T1 = h + SIGMA_1(e) + Ch(e, f, g) + K[t] + W[t];
        T2 = SIGMA_0(a) + Maj(a,b,c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }
    ctx->hash[0] += a;
    ctx->hash[1] += b;
    ctx->hash[2] += c;
    ctx->hash[3] += d;
    ctx->hash[4] += e;
    ctx->hash[5] += f;
    ctx->hash[6] += g;
    ctx->hash[7] += h;
}
void hash(sha256_context* ctx, byte* data)
{
    uint32_t n = ctx->dataLength & 0x3f;
    for(size_t i=0; i< ctx->dataLength - n; i+=64)
    {
        parse(ctx, data + i);
        update(ctx);
    }
    byte buffer[128] = {0};
    memcpy(buffer, data + ctx->dataLength - n, n);
    uint32_t N = pad(buffer, ctx->dataLength);
    for (int i = 0; i < N; i+=64)
    {
        parse(ctx, buffer+i);
        update(ctx);
    }
}
void hash_file(sha256_context* ctx, FILE* file)
{
    byte buffer[128] = {0};
    uint32_t n = fread(buffer, 1, 64, file);
    while(n == 64)
    {
        parse(ctx, buffer);
        update(ctx);
        ctx->dataLength += n;
        n = fread(buffer, 1, 64, file);
    }
    ctx->dataLength += n;
    uint32_t N = pad(buffer, ctx->dataLength);
    for (int i = 0; i < N; i+=64)
    {
        parse(ctx, buffer+i);
        update(ctx);
    }
}
void finish(sha256_context* ctx, byte output[32])
{
    for(int i=0; i<8; i++)
    {
        for(int j=3; j>=0; j--)
        {
            output[4*i + j] = ctx->hash[i] & 0xff;
            ctx->hash[i] >>= 8;
        }
    }
}
void sha256_hash(const void* data, size_t len, byte output[32])
{
    sha256_context ctx;
    init(&ctx, len);
    hash(&ctx, (byte*)data);
    finish(&ctx, output);
}
void sha256_hash_file(FILE* file, byte output[32])
{
    sha256_context ctx;
    init(&ctx, 0);
    hash_file(&ctx, file);
    finish(&ctx, output);
}
