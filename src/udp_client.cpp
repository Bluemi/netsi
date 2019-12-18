#ifndef __NETSI_UDP_CLIENT_CLASS__
#define __NETSI_UDP_CLIENT_CLASS__

#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "peer.hpp"
#include "client_network_manager.hpp"

using boost::asio::ip::udp;

class game {
	public:
		game() : _client_network_manager(), _peer() {}

		void run() {
			udp::endpoint init_endpoint = _client_network_manager.resolve("localhost", "1350");
			_peer = _client_network_manager.create_peer(init_endpoint);

			_client_network_manager.run();

			_peer->async_send("hello from client");

			_client_network_manager.join();
		}
	private:
		client_network_manager _client_network_manager;
		std::shared_ptr<peer> _peer;
};

int main()
{
	game game;
	game.run();

	return 0;
}

#endif
