#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "peer.hpp"

using boost::asio::ip::udp;

int main(int argc, const char** argv)
{
	if (argc != 2) {
		std::cout << "usage: client MESSAGE" << std::endl;
		return 1;
	}

	boost::asio::io_context io_context;

	udp::resolver resolver(io_context);
	udp::endpoint init_endpoint = *resolver.resolve(udp::v4(), "localhost", "1350").begin();

	peer p(io_context, init_endpoint);
	p.contact_remote();

	// receive
	p.init_by_response();

	std::cout << "local data endpoint: " << p.get_socket().local_endpoint() << std::endl;
	std::cout << "remote data endpoint: " << p.get_socket().remote_endpoint() << std::endl;

	std::string send_buf = { argv[1] };
	std::cout << "start send" << std::endl;
	p.async_send(send_buf);
	std::cout << "sending " << send_buf << " to " << p.get_socket().remote_endpoint() << std::endl;

	return 0;
}
