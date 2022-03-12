#include <Node.hpp>

namespace Basalt
{

    void Node::updateSamples(const Array<NodeId>& candidates){
         for(uint32_t i=0; i<_view.size() ; ++i){
              uint32_t currentHash = RankFunc(_view[i].seed, _view[i].id);
             for (NodeId p : candidates) {
                if(_view[i].id == p){ _view[i].hits++; return; }
                uint32_t P_Hash = RankFunc(_view[i].seed, _view[i].id);
                if(_view[i].id == nullptr || P_Hash < currentHash){
                    currentHash = P_Hash;
                    _view[i].hits = 1;
                    _view[i].id = p;
                }
             }
         }

    }


    NodeId Node::selectPeer(){
        uint32_t p=0;
        for(uint32_t i=1; i<_view.size(); ++i)
            p = _view[p].hits<_view[i].hits? p:i;
        _view[p].hits++;
        return _view[p].id;
    }  

    
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
