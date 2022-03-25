#include <Node.hpp>

namespace Basalt
{
	void Node::updateSamples(const Array<NodeId>& candidates){
		for(uint32_t i=0; i<_view.size() ; ++i){
			Hash<16>&& currentHash = _rankingFunc(_view[i].id, _view[i].seed);
			for (const NodeId& p : candidates) {
				if(_view[i].id == p){ _view[i].hits++; continue; }
				Hash<16>&& P_Hash = _rankingFunc(_view[i].id, _view[i].seed);
				if(_view[i].id == NodeId::null() || P_Hash < currentHash){
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
	void Node::update() {
		NodeId p = selectPeer();
		NodeId q = selectPeer();
		pull(p);
		push(q);
	}
	uint32_t Node::generateSeed() {
		std::uniform_int_distribution<uint32_t> dist;
		return dist(_rng);
	}
	Array<NodeId> Node::reset() {
		Array<NodeId> samples(_k);
		size_t v = _view.size();
		for (size_t i = 0; i < _k; i++) {
			_r = (_r + 1) % v; // get the next index and wrap around
			samples[i] = _view[_r].id;
			_view[_r].seed = generateSeed();
		}
		Array<NodeId> candidates(_view.size());
		for(uint32_t i=0; i<_view.size(); ++i)
			candidates[i] = _view[i].id;
		updateSamples(candidates);
		return samples;
	}
	void Node::on_pull(net::Message& req){
		// put our view in the response
	}
	void Node::on_push(net::Message& req){
		// update our view, and make a PULL_RESP respoonse
	}
	Node::Node(NodeId id, const Array<NodeId>& bs, uint32_t k, Hash<16> (*h)(const NodeId&, uint32_t),
		bool isByzantine, bool isSgx): 
		_id(id), _isByzantine(isByzantine), _isSGX(isSgx),_rankingFunc(h), _k(k)
	{
		_view = Array<ViewEntry>(bs.size());
		for(ViewEntry& e: _view){
			e.hits = 0;
			e.seed = generateSeed();
			e.id = NodeId::null();
		}
		updateSamples(bs);
	}
		
} // namespace Basalt
