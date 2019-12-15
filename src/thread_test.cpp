#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class client_handler {
	public:
		client_handler(
				boost::asio::io_context& io_context,
				udp::endpoint& remote_endpoint,
				const std::string& client_name
		)
			: _io_context(&io_context), _client_name(client_name), _remote_endpoint(remote_endpoint)
		{ }

		void operator()() {
			udp::socket socket(*_io_context);
			socket.connect(_remote_endpoint);
			std::cout << "handler for \"" << _client_name << "\" online\n\t";
			std::cout << "local endpoint: " << socket.local_endpoint() << std::endl;

			while (true) {
				boost::array<char, 256> recv_buf;
				boost::system::error_code error;

				long len = socket.receive(boost::asio::buffer(recv_buf), 0, error);

				if (error) {
					std::cout << "unknown client handler error";
					return;
				}

				std::string message(recv_buf.cbegin(), recv_buf.cbegin()+len);
				std::cout << "received message:\n\tsender: " << _client_name << "\n\tmessage: " << message;
				std::string response = message + " acknowlegded";
				boost::system::error_code ignored_error;

				socket.send_to(boost::asio::buffer(response), _remote_endpoint, 0, ignored_error);
			}
			socket.close();
		}
	private:
		boost::asio::io_context* _io_context;
		std::string _client_name;
		udp::endpoint _remote_endpoint;
};

int main() {
	boost::asio::io_context io_context;
	udp::socket socket(io_context, udp::endpoint(udp::v4(), 1350));

	while (true) {
		boost::array<char, 256> recv_buf;
		udp::endpoint remote_endpoint;
		boost::system::error_code error;

		long len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);

		if (error) {
			std::cout << "error, TODO make better :)" << std::endl;
		}

		std::string client_name(recv_buf.cbegin(), recv_buf.cbegin()+len);
		std::cout << "client connected: " << client_name << std::endl;

		// TODO

		client_handler client_handler(io_context, remote_endpoint, client_name);
		boost::thread client_handler_thread(client_handler);
	}

	return 0;
}
