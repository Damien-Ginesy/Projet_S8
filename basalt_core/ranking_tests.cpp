#include <SpookyHash.h>
#include <iostream>
#include <time.h>
#include <random>
#include <SHA256Hash.hpp>


struct View
{
    static constexpr size_t N = 32;
    struct Entry { uint16_t id; uint32_t seed; };
    Entry array[N];
    View(){
        std::random_device rng;
        std::uniform_int_distribution<uint32_t> dist;
        for (size_t i=0; i<N; ++i)
        {
            array[i].id = i;
            array[i].seed = dist(rng);
        }

    }
    Entry* begin() { return array; }
    Entry* end() { return array + N; }
    const Entry* begin() const { return array; }
    const Entry* end() const { return array + N; }
    Entry& operator[](size_t i) { return array[i]; }
    const Entry& operator[](size_t i) const { return array[i]; }
};
std::ostream& operator<<(std::ostream& s, const View::Entry& e){
    s << "{id: " << e.id << ", seed: " << e.seed << '}';
    return s;
}

template<size_t S>
size_t findMin(const View& view, size_t index, Hash<S> (*h)(uint16_t, uint32_t)){
    size_t res = 0;
    Hash<S>&& hMin = h(view[0].id, view[0].seed);
    for (size_t i = 1; i < View::N; i++)
    {
        Hash<S>&& hCur = h(view[i].id, view[index].seed);
        if(hCur < hMin){
            res = i;
            hMin = hCur;
        }
    }
    return res;
}
Hash<16> spooky_ranking(uint16_t id, uint32_t seed){
    uint8_t data[] = {(uint8_t)(id & 255), (uint8_t)(id >> 8)};
    return SpookyHash(data, 2, seed);
}

Hash<32> sha_ranking(uint16_t id, uint32_t seed){
    uint8_t data[] = {
        (uint8_t)(id & 255),
        (uint8_t)(id >> 8),
        (uint8_t)(seed & 255),
        (uint8_t)((seed>>8) & 255),
        (uint8_t)((seed>>16) & 255),
        (uint8_t)((seed>>24) & 255)
    };
    return SHA256Hash(data, 6);
}
int main(int argc, char const *argv[])
{
    View v;
    for(auto& e: v)
        std::cout << e << '\n';
    std::cout << "SpookyHash test\n======\n";
    for (size_t i = 0; i < View::N; i++){
        size_t match = findMin<16>(v, i, spooky_ranking);
        printf("%" PRIuMAX " ", match);
    }
    std::cout << "\nSHA256 test\n======\n";
    for (size_t i = 0; i < View::N; i++){
        size_t match = findMin<32>(v, i, sha_ranking);
        printf("%" PRIuMAX " ", match);
    }
    std::cout << '\n';
}
