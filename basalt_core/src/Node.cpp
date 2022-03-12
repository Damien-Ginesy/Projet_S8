#include <Node.hpp>

namespace Basalt
{
    void Node::update(Hash<HashSize> (*h)(const NodeId&, uint32_t seed)) {
      NodeId p = selectPeer();
      NodeId q = selectPeer();
      pull(p);
      push(q); //+ _view
    }


    Array<NodeId> Node::reset() {
      Array<NodeId> samples = new Array<NodeId>(_k);
      size_t v = _view.size();
      for (size_t i = 0; i < _k; i++) {
        _r = (_r%(v-1))+1;
        samples[i] = _view[_r].id;
        _view[_r].seed = randSeed();
      }
      updateSamples(_view);
      return samples;
    }

    
} // namespace Basalt
