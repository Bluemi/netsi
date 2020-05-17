#include <iostream>
#include <vector>
#include <netsi/client.hpp>
#include <unistd.h>

constexpr std::size_t BUFFER_SIZE = 2048;

int main() {
	netsi::client_network_manager cnm(BUFFER_SIZE);
	netsi::endpoint init_endpoint = cnm.resolve(std::string("localhost"), 1350);
	std::cout << "init endpoint: " << init_endpoint << std::endl;
	netsi::peer peer = cnm.create_peer(init_endpoint);

	bool running = false;

	peer.send(std::string("hello"));

	usleep(1000000);

	/*
	while (running) {

		while (peer.has_message()) {
			const std::vector<char>& buffer = peer.pop_message();
			std::cout << std::string(buffer.cbegin(), buffer.cend()) << std::endl;
		}
	}
	*/
}
