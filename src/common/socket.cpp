#include "socket.hpp"

namespace netsi {
	socket_impl::socket_impl(std::uint16_t port) : socket(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)) {}

	void socket_impl::run() {
		run_thread = boost::thread(std::ref(*this));
	}

	void socket_impl::operator()() {
		io_context.run();
	}

	void socket_impl::stop() {
		io_context.stop();
		if (run_thread) {
			run_thread->join();
		}
	}
}
