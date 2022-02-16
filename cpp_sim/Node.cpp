#include "Node.hpp"


namespace rps
{
    Array<uint32_t> sample_n(size_t n, const rps::ArrayView<rps::Node>& nodes){
        rps::Array<uint32_t> samples(n);
        std::random_device rng;
        std::uniform_int_distribution<uint32_t> dist(0, nodes.size()-1);
        samples[0] = dist(rng);
        SAMPLE: for(size_t i=1; i<n; i++){
            size_t x=dist(rng);
            for(size_t j=0; j<i; j++)
                if(samples[j] == x) goto SAMPLE;
            samples[i] = x;
        }
        return samples;
    }
    uint32_t Node::genSeed()
    {
        std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);
        return dist(_rng);
    }
    /* constructors/assignment */
    Node::Node(uint32_t viewSize, bool (*r)(uint32_t, uint32_t, uint32_t)): _viewSize(viewSize), _rankingFunction(r)
    {
        _view = Array<uint32_t>(viewSize, (uint32_t)-1);
        _hits = new uint32_t[viewSize]{0};
        _seeds = new uint32_t[viewSize];
        for(uint32_t i=0; i<viewSize; ++i)
            _seeds[i] = genSeed();
    }
    Node::Node(Node&& other): _r(other._r), id(other.id), _viewSize(other._viewSize), _view(other._view),
        _seeds(other._seeds), _hits(other._hits), _rankingFunction(other._rankingFunction), isByzantine(other.isByzantine)
    {
        std::cout << "move" << std::endl;
        other._seeds = other._hits = nullptr;
    }
    Node::~Node() { delete[] _hits; delete[] _seeds; }
    

    /* accessors */
    uint32_t Node::seeds(size_t i) const { return _seeds[i]; }
    uint32_t Node::operator[] (size_t i) const { return _view[i]; }
    uint32_t Node::viewSize() const { return _viewSize; }


} // namespace basalt


