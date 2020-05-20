#ifndef __NETSI_CLIENT_NETWORK_MANAGER_CLASS__
#define __NETSI_CLIENT_NETWORK_MANAGER_CLASS__

#include <optional>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../common/endpoint.hpp"
#include "../common/peer.hpp"
#include "../common/socket.hpp"

namespace netsi {

	class ClientNetworkManager {
		public:
			ClientNetworkManager(std::size_t buffer_size);
			~ClientNetworkManager();
			Endpoint resolve(const std::string& remote_host, const std::uint16_t& port);
			Peer create_peer(const Endpoint& init_endpoint);
		private:
			void start_receive();
			void handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred);

			SocketPtr _socket;
			std::optional<Peer> _peer;
			std::vector<char> _receive_buffer;
			Endpoint _remote_endpoint;
			std::size_t _buffer_size;
	};
}

#endif
