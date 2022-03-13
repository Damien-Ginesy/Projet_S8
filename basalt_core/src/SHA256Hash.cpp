#include <SHA256Hash.hpp>
#include <sha256.h>

SHA256Hash::SHA256Hash(const void* in, size_t len){
    sha256_hash(in, len, _data);
}