#ifndef __NETSI_PEER_CLASS__
#define __NETSI_PEER_CLASS__

#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace netsi {
	using boost::asio::ip::udp;

	class peer {
		public:
			peer();
			peer(boost::asio::io_context& io_context);

			void send(const std::vector<char>& buffer);
			void send(const std::string& buffer);

			bool has_message() const;
			std::vector<char> pop_message();

			void disconnect();
	};
}

#endif
