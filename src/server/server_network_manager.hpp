#ifndef __NETSI_SERVER_NETWORK_MANAGER_CLASS__
#define __NETSI_SERVER_NETWORK_MANAGER_CLASS__

#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../util/blocking_queue.hpp"
#include "../common/endpoint.hpp"
#include "../common/peer.hpp"
#include "../common/socket.hpp"

namespace netsi {
	struct ClientRequest {
		ClientRequest(std::vector<char> m, const Endpoint& re) : message(m), remote_endpoint(re) {}
		std::vector<char> message;
		Endpoint remote_endpoint;
	};

	class ServerNetworkManager {
		public:
			ServerNetworkManager(const std::uint16_t port, const std::size_t buffer_size);
			~ServerNetworkManager();

			bool has_client_request() const;
			ClientRequest pop_client_request();

			Peer create_peer(const Endpoint& remote_endpoint);

			boost::asio::io_context& get_context();
		private:
			void start_receive();
			void handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred);

			SocketPtr _socket;
			std::vector<char> _receive_buffer;
			std::unordered_map<Endpoint, Peer, EndpointHasher> _peers;
			std::unordered_map<Endpoint, BlockingQueue<std::vector<char>>, EndpointHasher> _pending_peers;
			Endpoint _remote_endpoint;
			BlockingQueue<ClientRequest> _client_requests;
			std::size_t _buffer_size;
	};
}

#endif
