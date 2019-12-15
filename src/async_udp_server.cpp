#include<ctime>
#include<iostream>
#include<string>
#include<boost/array.hpp>
#include<boost/bind.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/asio.hpp>

using boost::asio::ip::udp;

class udp_server {
	public:
		udp_server(boost::asio::io_context& io_context) : _socket(io_context, udp::endpoint(udp::v4(), 1350)) { }

		void start_receive() {
			_socket.async_receive_from(
				boost::asio::buffer(_recv_buffer),
				_remote_endpoint,
				boost::bind(
					&udp_server::handle_receive,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
		}
	private:
		void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
			std::cout << "handle_receive():" << std::endl;
			std::cout << "\tmessage          : " << _recv_buffer.c_array() << std::endl;
			std::cout << "\tbytes transferred: " << bytes_transferred << std::endl;
			std::cout << "\terror code       : " << error << std::endl;
			std::cout << "\tendpoint         : " << _remote_endpoint << std::endl;

			if (!error || error == boost::asio::error::message_size) {
				boost::shared_ptr<std::string> message(new std::string("hey"));
				_socket.async_send_to(
					boost::asio::buffer(*message),
					_remote_endpoint,
					boost::bind(
						&udp_server::handle_send,
						this,
						message,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);
				start_receive();
			}
		}

		void handle_send(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes_transferred) {
			/*
			std::cout << "handle_send():" << std::endl;
			std::cout << "\tmessage: " << *message << std::endl;
			std::cout << "\terror  : " << error << std::endl;
			std::cout << "\tbytes transferred: " << bytes_transferred << std::endl;
			*/
		}

		udp::socket _socket;
		udp::endpoint _remote_endpoint;
		boost::array<char, 256> _recv_buffer;
};

int main() {
	try {
		boost::asio::io_context io_context;
		udp_server server(io_context);
		server.start_receive();
		io_context.run();
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
