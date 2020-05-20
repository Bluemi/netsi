#include "socket.hpp"

namespace netsi {
	Socket::Socket(std::uint16_t port) : socket(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)) {}

	void Socket::run() {
		run_thread = boost::thread(std::ref(*this));
	}

	void Socket::operator()() {
		io_context.run();
	}

	void Socket::stop() {
		io_context.stop();
		if (run_thread) {
			run_thread->join();
		}
	}
}
