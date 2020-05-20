#ifndef __NETSI_SOCKET_CLASS__
#define __NETSI_SOCKET_CLASS__

#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace netsi {
	class Socket {
		public:
			Socket();
			Socket(std::uint16_t port);

			void run();
			void operator()();

			void stop();

			boost::asio::io_context io_context;
			boost::asio::ip::udp::socket socket;
			std::optional<boost::thread> run_thread;
	};

	using SocketPtr = std::shared_ptr<Socket>;
}

#endif
