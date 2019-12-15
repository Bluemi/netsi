#ifndef __NETSI_PEER_CLASS__
#define __NETSI_PEER_CLASS__

#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::udp;

class peer {
	public:
		peer(boost::asio::io_context& io_context, const udp::endpoint& remote_endpoint) : _socket(io_context), _remote_endpoint(remote_endpoint) {
			boost::system::error_code error;
			_socket.connect(_remote_endpoint, error);

			if (error) {
				std::cout << "error while connecting remote peer" << std::endl;
			} else {
				std::cout << "new peer:" << std::endl;
				std::cout << "\tlocal endpoint : " << _socket.local_endpoint() << std::endl;
				std::cout << "\tremote endpoint: " << _socket.remote_endpoint() << std::endl;
			}
		}

		void init() {
			start_receive();
		}

		void init_by_response() {
			boost::array<char, 256> recv_buf;
			udp::endpoint remote_endpoint;
			_socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
			_socket.connect(remote_endpoint);
		}

		void start_receive() {
			std::cout << "start receive:" << std::endl;
			std::cout << "\tlocal_endpoint : " << _socket.local_endpoint() << std::endl;
			std::cout << "\tremote endpoint: " << _socket.remote_endpoint() << std::endl;
			_socket.async_receive(
				boost::asio::buffer(_recv_buffer),
				boost::bind(
					&peer::handle_receive,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
		}

		template<typename buffer_type>
		void async_send(const buffer_type& buffer) {
			_socket.async_send(
				boost::asio::buffer(buffer),
				boost::bind(
					&peer::handle_send,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
		}

		void contact_remote() {
			boost::array<char, 0> send_buffer;
			_socket.send(boost::asio::buffer(send_buffer));
		}

		const udp::socket& get_socket() const {
			return _socket;
		}
	private:
		void handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred) {
			if (!error_code) {
				std::cout << "handle_receive():" << std::endl;
				std::cout << "\tmessage           : " << _recv_buffer.c_array() << std::endl;
				std::cout << "\tbytes transferred : " << bytes_transferred << std::endl;
				std::cout << "\tremote endpoint   : " << _remote_endpoint << std::endl;
				std::cout << "\tremote endpoint(s): " << _socket.remote_endpoint() << std::endl;
				std::cout << "\tlocal  endpoint   : " << _socket.local_endpoint() << std::endl;
			} else {
				std::cout << "handle_receive() FAIL:" << std::endl;
				std::cout << "\terror code   : " << error_code << std::endl;
				std::cout << "\terror message: " << error_code.message() << std::endl;
			}
			start_receive();
		}

		void handle_send(const boost::system::error_code& error_code, std::size_t bytes_transferred) {
			if (!error_code) {
				std::cout << "handle_send():" << std::endl;
				std::cout << "\tmessage           : " << _recv_buffer.c_array() << std::endl;
				std::cout << "\tbytes transferred : " << bytes_transferred << std::endl;
				std::cout << "\tremote endpoint   : " << _remote_endpoint << std::endl;
				std::cout << "\tremote endpoint(s): " << _socket.remote_endpoint() << std::endl;
				std::cout << "\tlocal  endpoint   : " << _socket.local_endpoint() << std::endl;
			} else {
				std::cout << "handle send(): FAIL" << std::endl;
				std::cout << "\terror code: " << error_code << std::endl;
			}
		}

		udp::socket _socket;
		udp::endpoint _remote_endpoint;
		boost::array<char, 256> _recv_buffer;
};

#endif
