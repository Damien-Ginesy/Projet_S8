#include <Node.hpp>
#include <net/basalt_net.hpp>
#include <time.h>

namespace Basalt
{
	std::string Node::to_string() const{
		std::stringstream out;
		out << "{\"NodeID\":" << _id.to_string()
		<< ", \"malicieux\": " << _isByzantine 
		<< ", \"sgx\": " << _isSGX 
		<< ", \"age\": " << _iter
		<< ", \"vue\": [";
		for(size_t i=0; i<_view.size()-1; ++i)
			out << _view[i].to_string() << ", ";
		out << (_view.end()-1)->to_string() << "]}";
		return out.str();
	}
	std::string ViewEntry::to_string() const{
		std::stringstream out;
		out << "{\"NodeID\": " << id.to_string() << ", "
		"\"seed\": " << seed << ", \"hitCount\": " << hits
		<< '}';
		return out.str();
	}
	Array<ViewEntry>::View Node::view() const{
		return _view.view();
	}

	void Node::updateSamples(const Array<NodeId>& candidates){
		for(uint32_t i=0; i<_view.size() ; ++i){
			Hash_t&& currentHash = _rankingFunc(_view[i].id.id, _view[i].seed);
			for (const NodeId& p : candidates) {
				if(p == _id) continue;
				if(_view[i].id == p){ _view[i].hits++; continue; }
				Hash_t&& P_Hash = _rankingFunc(_view[i].id.id, _view[i].seed);
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
			p = _view[i].hits < _view[p].hits? i:p;
		_view[p].hits++;
		return _view[p].id;
	}  
	void Node::update() {
		NodeId p = selectPeer();
		NodeId q = selectPeer();
		pull(p);
		push(q);
		_iter++;
	}
	Array<NodeId> Node::reset() {
		std::cout << "Resetting " << _k << " nodes" << '\n';
		Array<NodeId> samples(_k);
		for (size_t i = 0; i < _k; i++) {
			_r = (_r + 1) % _view.size(); // get the next index and wrap around
			samples[i] = _view[_r].id;
			_view[_r].seed = _rng() & UINT32_MAX;
		}
		Array<NodeId> candidates(_view.size());
		for(uint32_t i=0; i<_view.size(); ++i)
			candidates[i] = _view[i].id;
		updateSamples(candidates);
		return samples;
	}
	void Node::pull(NodeId dest){
		net::Message req(net::PULL_REQ);
		req << _id;
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
		req << _id;
		for(const ViewEntry& e: _view)
			req << e.id;
		asio::ip::tcp::endpoint ep(dest._addr, dest._port);
		std::cout << "Attempting push to " << dest.to_string() << '\n';
		asio::error_code err = net::send_request(ep, req);
		if(err){
			std::cerr << "Coudln't send request: " << err.message() << '\n';
		}
	}
	void Node::on_pull_req(net::Message& req) const{
		// read sender
		NodeId sender;
		req >> sender;
		std::cout << "Received pull from " << sender.to_string() << '\n';
		// put our view in the response
		req << _id;
		for(const ViewEntry& e: _view)
			req << e.id;
		// add our own id
		req.set_type(net::PULL_RESP);
	}
	void Node::on_push_req(net::Message& req){
		// update our view, and make a PULL_RESP respoonse
		Array<NodeId> candidates(_view.size() + 1);
		for(NodeId* c=candidates.end()-1; c>=candidates.begin(); c--)
			req >> (*c);
		std::cout << "Received push from" << candidates[0].to_string() << '\n';
		updateSamples(candidates);
		req.set_type(net::PUSH_RESP);		
	}
	void Node::on_pull_resp(net::Message& resp){
		// read the view from the message and update
		Array<NodeId> candidates(_view.size() + 1);
		for(NodeId* c=candidates.end()-1; c>=candidates.begin(); c--)
			resp >> (*c);
		updateSamples(candidates);
		resp.set_type(net::SESSION_END);
	}
	using std::chrono::high_resolution_clock;
	Node::Node(NodeId id, const Array<NodeId>& bs, uint32_t k, RankFunc_t h,
		bool isByzantine, bool isSgx): 
		_id(id), _isByzantine(isByzantine), _isSGX(isSgx),_rankingFunc(h), _k(k),
		_rng(high_resolution_clock::now().time_since_epoch().count())
	{
		_view = Array<ViewEntry>(bs.size());
		for(ViewEntry& e: _view){
			e.hits = 0;
			e.seed = _rng() & UINT32_MAX;
			e.id = NodeId::null();
		}
		updateSamples(bs);
		for(const ViewEntry& e: _view)
			std::cout << e.to_string() << '\n';
	}
	
} // namespace Basalt
