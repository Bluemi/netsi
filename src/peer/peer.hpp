#ifndef __NETSI_PEER_CLASS__
#define __NETSI_PEER_CLASS__

#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../util/blocking_queue.hpp"

using boost::asio::ip::udp;

constexpr std::size_t BUFFER_SIZE = 256;

class peer {
	public:
		peer(boost::asio::io_context& io_context) : _socket(io_context, udp::v4()) {}

		/**
		 * Connects the underlying socket with the given remote endpoint
		 */
		boost::system::error_code connect(const udp::endpoint& remote_endpoint) {
			boost::system::error_code error;
			_socket.connect(remote_endpoint, error);
			return error;
		}

		void init() {
			start_receive();
		}

		void start_receive() {
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

		void async_send(const std::vector<char>& buffer) {
			std::shared_ptr<std::vector<char>> message(new std::vector<char>(buffer));

			_socket.async_send(
				boost::asio::buffer(*message),
				boost::bind(
					&peer::handle_send,
					this,
					message,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
		}

		udp::endpoint listen_for_kick_off() {
			boost::array<char, BUFFER_SIZE> recv_buf;
			udp::endpoint remote_endpoint;
			_socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
			return remote_endpoint;
		}

		void send_kick_off() {
			boost::array<char, 0> send_buffer;
			_socket.send(boost::asio::buffer(send_buffer));
		}

		void send_kick_off(const udp::endpoint& remote_endpoint) {
			boost::array<char, 0> send_buffer;
			_socket.send_to(boost::asio::buffer(send_buffer), remote_endpoint);
		}

		const udp::socket& get_socket() const {
			return _socket;
		}

		blocking_queue<std::vector<char>>& messages() {
			return _message_queue;
		}
	private:
		void handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred) {
			if (!error_code) {
				_message_queue.push(
					std::vector<char>(
						_recv_buffer.cbegin(),
						_recv_buffer.cbegin()+bytes_transferred
					)
				);
			} else {
				std::cout << "handle_receive() FAIL:" << std::endl;
				std::cout << "\terror code     : " << error_code << std::endl;
				std::cout << "\terror message  : " << error_code.message() << std::endl;
				std::cout << "\tlocal endpoint : " << _socket.local_endpoint() << std::endl;
				std::cout << "\tremote endpoint: " << _socket.remote_endpoint() << std::endl;
			}
			start_receive();
		}

		void handle_send(const std::shared_ptr<std::vector<char>> /*message*/, const boost::system::error_code& error_code, std::size_t /*bytes_transferred*/) {
			if (error_code) {
				std::cout << "handle send(): FAIL" << std::endl;
				std::cout << "\terror code: " << error_code << std::endl;
			}
		}

		udp::socket _socket;
		boost::array<char, BUFFER_SIZE> _recv_buffer;
		blocking_queue<std::vector<char>> _message_queue;
};

#endif