#ifndef __NETSI_SERVER_CLASS__
#define __NETSI_SERVER_CLASS__

#include <netsi/server/server_network_manager.hpp>
#include <netsi/util/cycle.hpp>

class game {
	public:
		void init() {
			_server_network_manager.run(1350);
		}

		void run() {
			std::cout << "game is running" << std::endl;

			for (netsi::cycle c(_server_network_manager.get_context(), boost::posix_time::milliseconds(40));; c.next()) {
				check_new_peers();
				handle_to_clients();
			}

			_server_network_manager.join();
			std::cout << "game terminated" << std::endl;
		}
	private:
		void check_new_peers() {
			if (!_server_network_manager.get_connecting_endpoints().empty()) {
				netsi::endpoint remote_endpoint = _server_network_manager.get_connecting_endpoints().pop();
				std::shared_ptr<netsi::peer> remote_peer = _server_network_manager.endpoint_to_peer(remote_endpoint);
				_peers.push_back(remote_peer);
				std::cout << "new peer " << remote_endpoint << std::endl;
			}
		}

		void handle_to_clients() {
			unsigned char client_id = 0;
			for (const std::shared_ptr<netsi::peer>& p : _peers) {
				if (!p->messages().empty()) {
					std::vector<char> m = p->messages().pop();
					m.push_back(' ');
					m.push_back('0' + client_id);

					for (std::shared_ptr<netsi::peer>& prs : _peers) {
						prs->async_send(m);
					}
				}

				client_id++;
			}
		}

		netsi::server_network_manager _server_network_manager;
		std::vector<std::shared_ptr<netsi::peer>> _peers;

};

int main() {
	game game;
	game.init();
	game.run();
	return 0;
}

#endif
