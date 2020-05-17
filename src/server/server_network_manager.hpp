#ifndef __NETSI_SERVER_NETWORK_MANAGER_CLASS__
#define __NETSI_SERVER_NETWORK_MANAGER_CLASS__

#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../util/endpoint.hpp"
#include "../common/peer.hpp"
#include "../common/socket.hpp"

namespace netsi {
	class server_network_manager {
		public:
			server_network_manager(const std::uint16_t port, const std::size_t buffer_size);
			~server_network_manager();

			bool has_new_client() const;
			endpoint pop_new_client();

			peer create_peer(const endpoint& remote_endpoint);
		private:
			void start_receive();
			void handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred);

			socket_ptr _socket;
			std::vector<peer> _peers;
			std::vector<char> _receive_buffer;
			boost::asio::ip::udp::endpoint _remote_endpoint;
			std::size_t _buffer_size;
	};
}

#endif
