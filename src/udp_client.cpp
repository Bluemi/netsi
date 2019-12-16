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

	peer p(io_context);
	p.send_kick_off(init_endpoint);

	// receive
	udp::endpoint remote_endpoint = p.listen_for_kick_off();
	p.connect(remote_endpoint);
	p.init();

	std::string send_buf(argv[1]);
	p.async_send(send_buf);

	io_context.run();

	return 0;
}
