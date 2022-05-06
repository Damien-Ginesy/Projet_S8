#include <Node.hpp>
#include <net/basalt_net.hpp>
#include <time.h>
#include <algorithm>
#include <random>

namespace Basalt
{
	std::string Node::to_string() const{
		std::stringstream out;
		out << "{\"nodeID\":" << _id.to_string()
		<< ", \"sgx\": " << (_isSGX? "true":"false")
		<< ", \"age\": " << (_iter? "true":"false")
		<< ", \"vue\": [";
		for(size_t i=0; i<_view.size()-1; ++i)
			out << _view[i].to_string() << ", ";
		out << (_view.end()-1)->to_string() << "]"
			<< ", \"malicieux\": " << IS_BYZANTINE << "}";

		return out.str();
	}
	std::string ViewEntry::to_string() const{
		std::stringstream out;
		out << "{\"nodeID\": " << id.to_string() << ", "
		"\"seed\": " << seed << ", \"hitCount\": " << hits
		<< '}';
		return out.str();
	}
	Array<ViewEntry>::View Node::view() const{
		return _view.view();
	}
	using std::chrono::high_resolution_clock;

	#if IS_BYZANTINE==0
	void Node::updateSamples(const Array<NodeId>& candidates){
		for(uint32_t i=0; i<_view.size() ; ++i){
			Hash_t currentHash = _rankingFunc(_view[i].id.id, _view[i].seed);
			for (const NodeId& p : candidates) {
				if(p.id == _id.id) continue;
				if(_view[i].id.id == p.id){ _view[i].hits++; continue; }
				Hash_t P_Hash = _rankingFunc(p.id, _view[i].seed);
				if(_view[i].id == NodeId::null() || P_Hash < currentHash){
					currentHash = P_Hash;
					_view[i].hits = 1;
					_view[i].id = p;
				}
			}
		}
	}
	Array<NodeId> Node::reset() {
		std::cout << "Resetting " << _k << " nodes" << '\n';
		Array<NodeId> samples(_k);
		for (size_t i = 0; i < _k; i++) {
			_r = (_r + 1) % _view.size(); // get the next index and wrap around
			samples[i] = _view[_r].id;
			_view[_r].seed = _rng();
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
	void Node::on_push_req(net::Message& req, const asio::ip::tcp::endpoint& sender){
		// update our view, and make a PULL_RESP respoonse
		Array<NodeId> candidates(_view.size() + 1);
		for(NodeId* c=candidates.end()-1; c>=candidates.begin(); c--)
			req >> (*c);
		candidates[0]._addr = sender.address().to_v4();
		updateSamples(candidates);
		req.set_type(net::PUSH_RESP);
	}
	void Node::on_pull_resp(net::Message& resp, const asio::ip::tcp::endpoint& sender){
		// read the view from the message and update
		Array<NodeId> candidates(_view.size() + 1);
		for(NodeId* c=candidates.end()-1; c>=candidates.begin(); c--)
			resp >> (*c);
		candidates[0]._addr = sender.address().to_v4();
		updateSamples(candidates);
		resp.set_type(net::SESSION_END);
	}
	Node::Node(NodeId id, const Array<NodeId>& bs, uint32_t k, RankFunc_t h,
		bool isByzantine, bool isSgx):
		_id(id), _isSGX(isSgx),_rankingFunc(h), _k(k)
	{
		_view = Array<ViewEntry>(bs.size());
		for(ViewEntry& e: _view){
			e.hits = 0;
			e.seed = _rng();
			e.id = NodeId::null();
		}
		updateSamples(bs);
		for(const ViewEntry& e: _view)
			std::cout << e.to_string() << '\n';
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
	#else
	Node::Node(NodeId id, const Array<NodeId>& bs, const Array<NodeId>& friends):
		_id(id)
	{
		_friends = friends;
		_view = Array<ViewEntry>(bs.size());
		for(size_t i=0; i<bs.size(); ++i)
			_view[i] = ViewEntry{bs[i], _rng(), 0};
	}
	void Node::push(NodeId dest){
		net::Message req(net::PUSH_REQ);
		Array<NodeId> candidates(_view.size());
		std::random_device rng;
		std::sample(_friends.begin(), _friends.end(), candidates.begin(), candidates.size(), rng);
		req << _id;
		for(const NodeId& i: candidates) req << i;
		asio::ip::tcp::endpoint ep(dest._addr, dest._port);
		std::cout << "Attempting push to " << dest.to_string() << '\n';
		asio::error_code err = net::send_request(ep, req);
		if(err){
			std::cerr << "Coudln't send request: " << err.message() << '\n';
		}
	}

	#endif
	NodeId Node::selectPeer(){
		uint32_t p=0;
		for(uint32_t i=1; i<_view.size(); ++i)
			p = _view[i].hits < _view[p].hits? i:p;
		_view[p].hits++;
		return _view[p].id;
	}
	void Node::update() {
		NodeId p = selectPeer();
		#if IS_BYZANTINE==0
		NodeId q = selectPeer();
		pull(p);
		push(q);
		#else
		push(p);
		#endif
		_iter++;
	}
	#if IS_BYZANTINE==0
	void Node::on_pull_req(net::Message& req) const{
		// read sender
		NodeId sender;
		req >> sender;
		// put our view in the response
		req << _id;
		for(const ViewEntry& e: _view)
			req << e.id;
		// add our own id
		req.set_type(net::PULL_RESP);
	}
	#else
	void Node::on_pull_req(net::Message& req) const{
		// read sender
		NodeId sender;
		req >> sender;
		std::cout << "Received pull from " << sender.to_string() << '\n';
		// add our own id
		req << _id;
		Array<NodeId> candidates(_view.size());
		std::random_device rng;
		std::sample(_friends.begin(), _friends.end(), candidates.begin(), candidates.size(), rng);
		// put our view in the response
		for(const NodeId& i: candidates) req << i;
		req.set_type(net::PULL_RESP);
	}
	#endif

} // namespace Basalt
