#include "client_network_manager.hpp"

#include <iostream>

namespace netsi {
	client_network_manager::client_network_manager(std::size_t buffer_size)
		: _socket(std::make_shared<socket_impl>(5555)), _receive_buffer(buffer_size), _buffer_size(buffer_size)
	{
		start_receive();
		_socket->run();
	}

	client_network_manager::~client_network_manager() {
		_socket->stop();
	}

	endpoint client_network_manager::resolve(const std::string& remote_host, const std::uint16_t& port) {
		boost::asio::ip::udp::resolver resolver(_socket->io_context);
		std::string port_string = std::to_string(port);
		return *resolver.resolve(udp::v4(), remote_host, port_string.c_str()).begin();
	}

	peer client_network_manager::create_peer(const endpoint& init_endpoint) {
		return peer(_socket, init_endpoint);
	}

	void client_network_manager::start_receive() {
		_socket->socket.async_receive_from(
			boost::asio::buffer(_receive_buffer),
			_remote_endpoint,
			boost::bind(
				&client_network_manager::handle_receive,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}

	void client_network_manager::handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred) {
		std::cout << "client got message" << std::endl;
		std::cout << " message: " << std::string(_receive_buffer.cbegin(), _receive_buffer.cbegin() + bytes_transferred) << std::endl;
		std::cout << " error code: " << error_code << std::endl;
		std::cout << " endpoint: " << _remote_endpoint << std::endl;
		std::cout << " bytes transferred: " << bytes_transferred << std::endl;

		start_receive();
	}
}
