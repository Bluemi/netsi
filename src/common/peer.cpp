#include "peer.hpp"

#include <iostream>

namespace netsi {
	peer::peer(socket_ptr socket, boost::asio::ip::udp::endpoint remote_endpoint): _socket(socket), _remote_endpoint(remote_endpoint) {}

	bool peer::has_message() const {
		return false;
	}

	std::vector<char> peer::pop_message() {
		return {};
	}

	void peer::handle_send(const std::shared_ptr<std::vector<char>> message, const boost::system::error_code& error_code, std::size_t bytes_transferred) {
		std::cout << "sent message" << std::endl;
		std::cout << "  error_code: " << error_code << std::endl;
		std::cout << "  bytes_transferred: " << bytes_transferred << std::endl;
		std::cout << "  remote endpoint: " << _remote_endpoint << std::endl;
	}
}
