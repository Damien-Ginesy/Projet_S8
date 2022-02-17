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
    /* private methods */
    uint32_t Node::genSeed()
    {
        std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);
        return dist(_rng);
    }
    void Node::updateView(ArrayView<uint32_t> candidates, const Node* sender){
        for(uint32_t i=0; i<_viewSize; ++i){
            uint32_t prevHash = _rankingFunction(_seeds[i], _view[i]);
            auto updateSample = [&](uint32_t candidate){
                if(_view[i] == candidate){ _hits[i]++; return; }
                uint32_t candHash = _rankingFunction(_seeds[i], candidate);
                if(_view[i] == ID_INVALID || candHash < prevHash){
                    prevHash = candHash;
                    this->_hits[i] = 1;
                    this->_view[i] = candidate;
                }
            };
            if(sender) updateSample(sender->id);
            for(uint32_t& c: candidates) updateSample(c);
        }
    }
    ArrayView<uint32_t> Node::pull() const {
        return _view.view();
    }
    void Node::push(ArrayView<uint32_t> view, const Node* sender){
        updateView(view, sender);
    }
    uint32_t Node::selectPeer() const {
        uint32_t p=0;
        for(uint32_t i=1; i<_viewSize; ++i)
            p = _hits[p]<_hits[i]? p:i;
        return _view[p];
    }
    /* public methods */
    void Node::init(uint32_t id, bool isByzatine, ArrayView<uint32_t> bootstrap){
        this->id = id; this->isByzantine = isByzantine;
        updateView(bootstrap);
    }
    void Node::step(ArrayView<Node> nodes){

    }
    /* step method for malicious nodes */
    void Node::step(ArrayView<Node> honestNodes, unsigned F){

    }
    void Node::reset(uint32_t k){
        for (uint32_t i = 0; i < k; i++)
        {
            _r = (_r+1) % _viewSize;
            _seeds[_r] = genSeed();
        }
        updateView(_view.view());
    }

    /* constructors & assignment */
    Node::Node(uint32_t viewSize, uint32_t (*r)(uint32_t, uint32_t)): _viewSize(viewSize), _rankingFunction(r)
    {
        _view = Array<uint32_t>(viewSize, ID_INVALID);
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
    Node& Node::operator=(Node&& other){
        _r = other._r; this->id = other.id; this->_viewSize = other._viewSize;
        this->_rankingFunction = other._rankingFunction; this->isByzantine=other.isByzantine;
        this->_view = other._view;
        std::swap(this->_seeds, other._seeds);
        std::swap(this->_hits, other._hits);
        return *this;
    }
    Node::~Node() { delete[] _hits; delete[] _seeds; }
    

    /* accessors */
    uint32_t Node::seeds(size_t i) const { return _seeds[i]; }
    uint32_t Node::operator[] (size_t i) const { return _view[i]; }
    uint32_t Node::viewSize() const { return _viewSize; }


} // namespace basalt


