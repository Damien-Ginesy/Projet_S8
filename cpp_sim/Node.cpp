#include "Node.hpp"


namespace rps
{
    uint32_t Node::genSeed()
    {
        std::uniform_int_distribution<uint32_t> dist(0, SEED_MAX);
        return dist(_rng);
    }    
    void Node::updateView(const Array<uint32_t>& candidates, const Node* sender){
        for(size_t i=0; i<_viewSize; i++){
            if(sender && sender->_id == _view[i]){
                _hits[i]++;
                continue;
            }
            else if(sender && (_view[i] == ID_INVALID || _rankingFunction(_seeds[i], sender->_id, _view[i]))){
                _view[i] = sender->_id;
                _hits[i] = 1;
            }
            for(uint32_t c: candidates){
                if(_view[i] == c){
                    _hits[i]++;
                    break;
                }
                else if (_view[i] == ID_INVALID || _rankingFunction(_seeds[i], c, _view[i]))
                {
                    _view[i] = c;
                    _hits[i] = 1;
                }
            }
        }
    }
    Node::Node(Node&& other): _id(other._id), _viewSize(other._viewSize), 
        _view(other._view), _seeds(other._seeds), _hits(other._hits), 
        _rankingFunction(other._rankingFunction)
    {
        _r = other._r;
        other._seeds = other._hits = nullptr;
    }
    Node::~Node() {
        delete[] _seeds;
        delete[] _hits;
    }
    Node& Node::operator=(Node&& other){
        _view = other._view;
        _rankingFunction = other._rankingFunction;
        std::swap(_hits, other._hits);
        std::swap(_seeds, other._seeds);
        _r = other._r;
        _id = other._id;
        _viewSize = other._viewSize;
        return *this;
    }
    uint32_t Node::seeds(size_t i) const { return _seeds[i]; }
    const uint32_t Node::operator[] (size_t i) const { return _view[i]; }
    uint32_t Node::viewSize() const { return _viewSize; }
    uint32_t Node::id() const { return _id; }
    void Node::init(const Array<uint32_t>& bootstrap){
        updateView(bootstrap);
    }
    const Array<uint32_t>& Node::pull() const{
        return _view;
    }
    void Node::push(const Array<uint32_t>& view, Node* sender){
        updateView(view, sender);
    }
    uint32_t Node::selectPeer() {
        uint32_t* candidates = new uint32_t[_viewSize];
        uint32_t count = 0;
        uint32_t minHits = _hits[0];
        for(uint32_t i=1; i<_viewSize; i++)
            minHits = minHits<_hits[i]? minHits:_hits[i];
        for(uint32_t i=0; i<_viewSize; i++)
            if(_hits[i] == minHits)
                candidates[count++] = i;
        std::uniform_int_distribution<uint32_t> dist(0, count-1);
        size_t index = candidates[dist(_rng)];
        _hits[index]++;
        delete[] candidates;
        return _view[index];
    }
    void Node::update(Array<Node>& nodes){
        uint32_t p = selectPeer();
        updateView(nodes[p].pull(), nodes.at(p));
        uint32_t q = selectPeer();
        nodes[q].push(this->_view, this);
    }
    void Node::reset(uint32_t k){
        for(uint32_t i=0; i<k; i++){
            _r = (_r+1) % _viewSize;
            _seeds[_r] = genSeed();
        }
        updateView(_view);
    }
} // namespace basalt


