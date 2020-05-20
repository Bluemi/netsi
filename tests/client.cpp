#include <iostream>
#include <vector>
#include <netsi/client.hpp>
#include <unistd.h>

constexpr std::size_t BUFFER_SIZE = 2048;

int main() {
	netsi::ClientNetworkManager cnm(BUFFER_SIZE);
	netsi::Endpoint init_endpoint = cnm.resolve(std::string("localhost"), 1350);
	netsi::Peer peer = cnm.create_peer(init_endpoint);

	bool running = true;

	peer.send(std::string("login message"));

	sleep(1);

	while (running) {
		while (peer.has_message()) {
			const std::vector<char>& buffer = peer.pop_message();
			std::cout << std::string(buffer.cbegin(), buffer.cend()) << std::endl;
		}

		peer.send("This is my message");
		sleep(1);
	}
}
