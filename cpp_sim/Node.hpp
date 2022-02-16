#pragma once
#include <inttypes.h>
#include "Array.hpp"
#include <vector>
#include <random>
#include "Hash.hpp"

#define SEED_MAX        UINT32_MAX
#define ID_INVALID      UINT32_MAX

namespace rps
{
    
    class Node final
    {
    private:
        /* data */
        uint32_t _r=0;
        uint32_t _viewSize;
        Array<uint32_t> _view;
        uint32_t* _seeds;
        uint32_t* _hits;
        bool(*_rankingFunction)(uint32_t, uint32_t, uint32_t);
        std::random_device _rng;

        
        /* private methods */
        uint32_t genSeed();
        void updateView(const ArrayView<uint32_t>& candidates, const Node* sender=nullptr);
        ArrayView<uint32_t> pull() const;
        void push(const ArrayView<uint32_t>& view, Node* sender);
        uint32_t selectPeer();
    public:
        uint32_t id;
        bool isByzantine;
        /* Constructors and assignement operators */
        Node(uint32_t viewSize, bool(*rankingFunction)(uint32_t, uint32_t, uint32_t)); 
        Node(){}
        Node(const Node&) = delete;
        Node(Node&&);
        ~Node();
        Node& operator=(const Node&) = delete;
        Node& operator=(Node&&);

        /* init method for honest nodes */
        void init(uint32_t id, bool isByzatine, const Array<uint32_t>& bootstrap);
        void step(ArrayView<Node> nodes);
        /* step method for malicious nodes */
        void step(ArrayView<Node> honestNodes, unsigned F);
        void reset(uint32_t k);

        /* accessors */
        uint32_t seeds(size_t i) const;
        uint32_t operator[] (size_t i) const;
        uint32_t viewSize() const;
    };
    Array<uint32_t> sample_n(size_t n, const rps::ArrayView<rps::Node>& nodes);
} // namespace basalt



