#ifndef __NETSI_SERVER_CLASS__
#define __NETSI_SERVER_CLASS__

#include <iostream>
#include <netsi/server.hpp>

constexpr std::size_t BUFFER_SIZE = 2048;
constexpr std::uint16_t PORT = 1351;

int main() {
	netsi::ServerNetworkManager snm(PORT, BUFFER_SIZE);
	std::vector<netsi::Peer> peers;

	bool running = true;

	while (running) {
		// handle new connections
		while (snm.has_client_request()) {
			std::cout << "adding new client" << std::endl;
			netsi::ClientRequest client_request = snm.pop_client_request();
			netsi::Peer new_peer = snm.create_peer(client_request.endpoint);
			peers.push_back(new_peer);
		}

		// handle new messages
		for (netsi::Peer& peer : peers) {
			while (peer.has_message()) {
				const std::vector<char> buffer = peer.pop_message();
				std::string s(buffer.cbegin(), buffer.cend());
				
				std::cout << "replying to \"" << s << "\"" << std::endl;

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
