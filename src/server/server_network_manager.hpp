#ifndef __NETSI_SERVER_NETWORK_MANAGER_CLASS__
#define __NETSI_SERVER_NETWORK_MANAGER_CLASS__

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../util/endpoint.hpp"
#include "../peer/peer.hpp"

namespace netsi {
	class server_network_manager {
		public:
			server_network_manager(const std::uint16_t port, const std::size_t buffer_size);

			bool has_new_client() const;
			endpoint pop_new_client();

			peer endpoint_to_peer(const endpoint& remote_endpoint);
	};
}

#endif
