#include "client_network_manager.hpp"

namespace netsi {
	client_network_manager::client_network_manager() {}
	endpoint client_network_manager::resolve(const std::string& remote_host, const std::uint16_t& port) {
		return endpoint();
	}

	peer client_network_manager::create_peer(const endpoint& init_endpoint, const std::size_t buffer_size) {
		return peer();
	}
}
