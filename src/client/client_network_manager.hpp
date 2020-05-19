#ifndef __NETSI_CLIENT_NETWORK_MANAGER_CLASS__
#define __NETSI_CLIENT_NETWORK_MANAGER_CLASS__

#include <optional>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../common/endpoint.hpp"
#include "../common/peer.hpp"
#include "../common/socket.hpp"

namespace netsi {

	class client_network_manager {
		public:
			client_network_manager(std::size_t buffer_size);
			~client_network_manager();
			endpoint resolve(const std::string& remote_host, const std::uint16_t& port);
			peer create_peer(const endpoint& init_endpoint);
		private:
			void start_receive();
			void handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred);

			socket_ptr _socket;
			std::optional<peer> _peer;
			std::vector<char> _receive_buffer;
			endpoint _remote_endpoint;
			std::size_t _buffer_size;
	};
}

#endif
