#ifndef __NETSI_CLIENT_NETWORK_MANAGER_CLASS__
#define __NETSI_CLIENT_NETWORK_MANAGER_CLASS__

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../peer/peer.hpp"
#include "../util/endpoint.hpp"

namespace netsi {
	using boost::asio::ip::udp;

	class client_network_manager {
		public:
			client_network_manager();
			endpoint resolve(const std::string& remote_host, const std::uint16_t& port);
			peer create_peer(const endpoint& init_endpoint, const std::size_t buffer_size);
		private:
			std::shared_ptr<boost::asio::io_context> _io_context;
			std::optional<boost::thread> _run_thread;
	};
}

#endif
