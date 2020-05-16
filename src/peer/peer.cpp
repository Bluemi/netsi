#include "peer.hpp"

namespace netsi {
	peer::peer() {}
	peer::peer(boost::asio::io_context& io_context) {}

	void peer::send(const std::vector<char>& buffer) {}
	void peer::send(const std::string& buffer) {}

	bool peer::has_message() const {}
	std::vector<char> peer::pop_message() {}

	void peer::disconnect() {}
}
