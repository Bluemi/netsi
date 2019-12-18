#ifndef __NETSI_SERVER_CLASS__
#define __NETSI_SERVER_CLASS__

#include "server/server_network_manager.hpp"
#include "util/cycle.hpp"

class game {
	public:
		void init() {
			_server_network_manager.run(1350);
		}

		void run() {
			std::cout << "game is running" << std::endl;

			for (cycle c(_server_network_manager.get_context(), boost::posix_time::milliseconds(2000));; c.next()) {
				if (!_server_network_manager.get_connecting_endpoints().empty()) {
					udp::endpoint remote_endpoint = _server_network_manager.get_connecting_endpoints().pop();
					std::shared_ptr<peer> remote_peer = _server_network_manager.endpoint_to_peer(remote_endpoint);
					_peers.push_back(remote_peer);
					std::cout << "new peer " << remote_endpoint << std::endl;
				} else {
					unsigned int client_id = 0;
					for (std::shared_ptr<peer> p : _peers) {
						p->async_send(std::string("hello  client ") + std::to_string(client_id));
						client_id++;
					}
				}
			}

			_server_network_manager.join();
			std::cout << "game terminated" << std::endl;
		}
	private:
		server_network_manager _server_network_manager;
		std::vector<std::shared_ptr<peer>> _peers;

};

int main() {
	game game;
	game.init();
	game.run();
	return 0;
}

#endif
