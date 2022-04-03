#include <Node.hpp>
#include <net/basalt_net.hpp>

namespace Basalt
{
	std::string Node::to_string() const{
		std::stringstream out;
		out << "{\"id\":" << _id.to_string()
		<< ", \"is_byzantine\": " << _isByzantine 
		<< ", \"is_sgx\": " << _isSGX 
		<< ", \"view\": [";
		for(size_t i=0; i<_view.size()-1; ++i)
			out << _view[i].to_string() << ", ";
		out << (_view.end()-1)->to_string() << "]}";
		return out.str();
	}
	std::string ViewEntry::to_string() const{
		std::stringstream out;
		out << "{\"id\": " << id.to_string() << ", "
		"\"seed\": " << seed << ", \"hits\": " << hits
		<< '}';
		return out.str();
	}
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
			p = _view[i].hits<_view[p].hits? i:p;
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
		std::cout << "Resetting " << _k << " nodes" << '\n';
		Array<NodeId> samples(_k);
		for (size_t i = 0; i < _k; i++) {
			_r = (_r + 1) % _view.size(); // get the next index and wrap around
			samples[i] = _view[_r].id;
			_view[_r].seed = generateSeed();
		}
		Array<NodeId> candidates(_view.size());
		for(uint32_t i=0; i<_view.size(); ++i)
			candidates[i] = _view[i].id;
		updateSamples(candidates);
		return samples;
	}
	void Node::pull(NodeId dest){
		net::Message req(net::PULL_REQ);
		using namespace asio::ip;
		tcp::endpoint ep(dest._addr, dest._port);
		std::cout << "Attempting pull to " << dest.to_string() << '\n';
		asio::error_code err = net::send_request(ep, req);
		if(err){
			std::cerr << "Coudln't send request: " << err.message() << '\n';
		}
	}
	void Node::push(NodeId dest){
		net::Message req(net::PUSH_REQ);
		for(const ViewEntry& e: _view)
			req << e.id;
		req << _id;
		asio::ip::tcp::endpoint ep(dest._addr, dest._port);
		std::cout << "Attempting push to " << dest.to_string() << '\n';
		asio::error_code err = net::send_request(ep, req);
		if(err){
			std::cerr << "Coudln't send request: " << err.message() << '\n';
		}
	}
	void Node::on_pull_req(net::Message& req) const{
		// put our view in the response
		for(const ViewEntry& e: _view)
			req << e.id;
		// add our own id
		req << _id;
		std::cout << "Received pull" << '\n';
		req.set_type(net::PULL_RESP);
	}
	void Node::on_push_req(net::Message& req){
		// update our view, and make a PULL_RESP respoonse
		Array<NodeId> candidates(_view.size() + 1);
		for (int i = candidates.size() - 1; i >= 0; i--)
			req >> candidates[i];
		std::cout << "Received push from" << (candidates.end()-1)->to_string() << '\n';
		updateSamples(candidates);
		req.set_type(net::PUSH_RESP);		
	}
	void Node::on_pull_resp(net::Message& resp){
		// read the view from the message and update
		Array<NodeId> candidates(_view.size() + 1);
		for (int i = candidates.size() - 1; i >= 0; i--)
			resp >> candidates[i];
		updateSamples(candidates);
		resp.set_type(net::SESSION_END);
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
