#ifndef __NETSI_SERVER_CLASS__
#define __NETSI_SERVER_CLASS__

#include <netsi/server.hpp>

constexpr std::size_t BUFFER_SIZE = 2048;
constexpr std::uint16_t PORT = 1035;

int main() {
	netsi::server_network_manager snm(PORT, BUFFER_SIZE);
	std::vector<netsi::peer> peers;

	bool running = true;

	while (running) {
		// handle new connections
		while (snm.has_new_client()) {
			netsi::endpoint new_client_endpoint = snm.pop_new_client();
			netsi::peer new_peer = snm.endpoint_to_peer(new_client_endpoint);
			peers.push_back(new_peer);
		}

		// handle new messages
		for (netsi::peer& peer : peers) {
			while (peer.has_message()) {
				const std::vector<char>& buffer = peer.pop_message();
				std::string s(buffer.cbegin(), buffer.cend());

				if (s == "quit") {
					running = false;
				} else {
					s = "you sent \"" + s + "\"";
				}
				peer.send(s);
			}
		}
	}
	return 0;
}

#endif
