#include "server_network_manager.hpp"

#include <iostream>

namespace netsi {
	server_network_manager::server_network_manager(const std::uint16_t port, const std::size_t buffer_size)
		: _socket(std::make_shared<socket_impl>(port)), _buffer_size(buffer_size), _receive_buffer(buffer_size)
	{
		start_receive();
		_socket->run();
	}

	server_network_manager::~server_network_manager() {
		_socket->stop();
	}

	bool server_network_manager::has_new_client() const {
		return false;
	}

	endpoint server_network_manager::pop_new_client() {
		return endpoint();
	}

	peer server_network_manager::create_peer(const endpoint& remote_endpoint) {
		return peer(_socket, remote_endpoint);
	}

	void server_network_manager::start_receive() {
		std::cout << "starting receive" << std::endl;
		_socket->socket.async_receive_from(
			boost::asio::buffer(_receive_buffer),
			_remote_endpoint,
			boost::bind(
				&server_network_manager::handle_receive,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}

	void server_network_manager::handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred) {
		std::cout << "server got message" << std::endl;
		std::cout << " message: " << std::string(_receive_buffer.cbegin(), _receive_buffer.cbegin() + bytes_transferred) << std::endl;
		std::cout << " error code: " << error_code << std::endl;
		std::cout << " endpoint: " << _remote_endpoint << std::endl;
		std::cout << " bytes transferred: " << bytes_transferred << std::endl;

		start_receive();
	}
}
