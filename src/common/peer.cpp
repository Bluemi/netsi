#include "peer.hpp"

namespace netsi {
	peer::peer(socket_ptr socket, endpoint remote_endpoint)
		: _socket(socket), _remote_endpoint(remote_endpoint), _messages(std::make_shared<blocking_queue<std::vector<char>>>())
	{}

	bool peer::has_message() const {
		return !_messages->empty();
	}

	std::vector<char> peer::pop_message() {
		return _messages->pop();
	}

	void peer::push_message(const std::vector<char> message) {
		_messages->push(message);
	}

	void peer::handle_send(const std::shared_ptr<std::vector<char>> message, const boost::system::error_code& error_code, std::size_t bytes_transferred) {
	}
}
