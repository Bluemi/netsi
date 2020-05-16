#include "server_network_manager.hpp"

namespace netsi {
	server_network_manager::server_network_manager(const std::uint16_t port, const std::size_t buffer_size) {}

	bool server_network_manager::has_new_client() const {
		return false;
	}
	endpoint server_network_manager::pop_new_client() {
		return endpoint();
	}

	peer server_network_manager::endpoint_to_peer(const endpoint& remote_endpoint) {
		return peer();
	}
}
