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
    class Node
    {
    protected:
        uint32_t _r=0;
        uint32_t _id;
        uint32_t _viewSize;
        Array<uint32_t> _view;
        uint32_t* _seeds;
        uint32_t* _hits;
        bool(*_rankingFunction)(uint32_t, uint32_t, uint32_t);
        std::random_device _rng;
        uint32_t genSeed();
        void updateView(const Array<uint32_t>& candidates, const Node* sender=nullptr);
        virtual const Array<uint32_t>& pull() const;
        virtual void push(const Array<uint32_t>& view, Node* sender);
        virtual uint32_t selectPeer();
    public:
        /* Constructors and assignement operators */
        Node(uint32_t viewSize, uint32_t id, bool(*rankingFunction)(uint32_t, uint32_t, uint32_t)): 
                _id(id), _viewSize(viewSize),_rankingFunction(rankingFunction)
        {
            _view = Array<uint32_t>((size_t)viewSize);
            _seeds = new uint32_t[viewSize];
            _hits = new uint32_t[viewSize];
            for(uint32 i=0; i<viewSize; ++i) {
                _seeds[i] = genSeed();
                _hits[i] = 0;
                _view[i] = ID_INVALID;
            }
        }
        Node(){}
        Node(Node&&);
        virtual ~Node();
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        Node& operator=(Node&& other);

        /* other stuff */
        void init(const Array<uint32_t>& bootstrap);
        virtual void update(Array<Node>& nodes);
        void reset(uint32_t k);

        /* accessors */
        uint32_t seeds(size_t i) const;
        const uint32_t operator[] (size_t i) const;
        uint32_t viewSize() const;
        uint32_t id() const;
    };
} // namespace basalt



