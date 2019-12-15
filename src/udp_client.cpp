#include<iostream>
#include<boost/array.hpp>
#include<boost/asio.hpp>

using boost::asio::ip::udp;

int main(int argc, const char** argv)
{
	if (argc != 2) {
		std::cout << "usage: client MESSAGE" << std::endl;
		return 1;
	}

	try {
		boost::asio::io_context io_context;

		udp::resolver resolver(io_context);
		udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "localhost", "1350").begin();

		udp::socket socket(io_context);
		socket.open(udp::v4());

		// send
		std::string send_buf = {argv[1]};
		socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

		// receive
		boost::array<char, 256> recv_buf;
		udp::endpoint sender_endpoint;
		size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

		std::cout << "received: ";
		std::cout.write(recv_buf.data(), len);
		std::cout << "\n\tfrom: " << sender_endpoint << std::endl;

	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
