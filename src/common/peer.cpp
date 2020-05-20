#include "peer.hpp"

namespace netsi {
	Peer::Peer(SocketPtr socket, Endpoint remote_endpoint)
		: _socket(socket), _remote_endpoint(remote_endpoint), _messages(std::make_shared<BlockingQueue<std::vector<char>>>())
	{}

	bool Peer::has_message() const {
		return !_messages->empty();
	}

	std::vector<char> Peer::pop_message() {
		return _messages->pop();
	}

	void Peer::push_message(const std::vector<char>& message) {
		_messages->push(message);
	}

	void Peer::handle_send(const std::shared_ptr<std::vector<char>> message, const boost::system::error_code& error_code, std::size_t bytes_transferred) {
	}
}
