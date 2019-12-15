#include<ctime>
#include<iostream>
#include<string>
#include<boost/array.hpp>
#include<boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}

int main() {
	try {
		boost::asio::io_context io_context;
		udp::socket socket(io_context, udp::endpoint(udp::v4(), 1350));

		while (true) {
			boost::array<char, 10> recv_buf;
			udp::endpoint remote_endpoint;
			boost::system::error_code error;

			long len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);

			if (error) {
				if (error != boost::asio::error::message_size) {
					throw boost::system::system_error(error);
				} else {
					std::cout << "message size overflow";
				}
			}

			std::string message(recv_buf.cbegin(), recv_buf.cbegin()+len);
			std::cout << "received message: " << message << std::endl;
			std::string response = message + " acknowlegded";
			boost::system::error_code ignored_error;

			socket.send_to(boost::asio::buffer(response), remote_endpoint, 0, ignored_error);
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

