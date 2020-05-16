#ifndef __UDP_CLIENT_CLASS__
#define __UDP_CLIENT_CLASS__

#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <netsi/client/client_network_manager.hpp>
#include <netsi/util/cycle.hpp>

constexpr std::size_t BUFFER_SIZE = 256;

class game {
	public:
		game() : _client_network_manager(), _peer(), _counter(0) {}

		void init() {
			netsi::endpoint init_endpoint = _client_network_manager.resolve("localhost", "1350");
			_peer = _client_network_manager.create_peer(init_endpoint);

			_client_network_manager.run();
		}

		void run() {
			for (netsi::cycle c(_client_network_manager.get_context(), boost::posix_time::milliseconds(40));; c.next()) {
				send_message();
				handle_response();
			}
		}

		void send_message() {
			if (_counter++ == 20) {
				const std::string s("hello from client");
				const std::vector<char> message(s.cbegin(), s.cend());
				_peer->async_send(message);
				_counter = 0;
			}
		}

		void handle_response() {
			if (!_peer->messages().empty()) {
				const std::vector<char> m = _peer->messages().pop();
				const std::string message(m.cbegin(), m.cend());
				std::cout << "message: " << message << std::endl;
			}
		}

		void join() {
			_client_network_manager.join();
		}
	private:
		netsi::client_network_manager<BUFFER_SIZE> _client_network_manager;
		std::shared_ptr<netsi::peer<BUFFER_SIZE>> _peer;
		unsigned int _counter;
};

int main()
{
	game game;
	game.init();
	game.run();
	game.join();

	return 0;
}

#endif
