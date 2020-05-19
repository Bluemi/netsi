#ifndef __NETSI_SERVER_NETWORK_MANAGER_CLASS__
#define __NETSI_SERVER_NETWORK_MANAGER_CLASS__

#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../util/blocking_queue.hpp"
#include "../common/endpoint.hpp"
#include "../common/peer.hpp"
#include "../common/socket.hpp"
#include "../common/endpoint_hasher.hpp"

namespace netsi {
	struct client_request {
		client_request(std::vector<char> m, endpoint re) : message(m), remote_endpoint(re) {}
		std::vector<char> message;
		endpoint remote_endpoint;
	};

	class server_network_manager {
		public:
			server_network_manager(const std::uint16_t port, const std::size_t buffer_size);
			~server_network_manager();

			bool has_client_request() const;
			client_request pop_client_request();

			peer create_peer(const endpoint& remote_endpoint);
		private:
			void start_receive();
			void handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred);

			socket_ptr _socket;
			std::vector<char> _receive_buffer;
			std::unordered_map<endpoint, peer, EndpointHasher> _peers;
			endpoint _remote_endpoint;
			blocking_queue<client_request> _client_requests;
			std::size_t _buffer_size;
	};
}

#endif
