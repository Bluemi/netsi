#include "client_network_manager.hpp"

namespace netsi {
	using udp = boost::asio::ip::udp;

	ClientNetworkManager::ClientNetworkManager(std::size_t buffer_size)
		: _socket(std::make_shared<Socket>()), _receive_buffer(buffer_size), _buffer_size(buffer_size)
	{
		start_receive();
		_socket->run();
	}

	ClientNetworkManager::~ClientNetworkManager() {
		_socket->stop();
	}

	Endpoint ClientNetworkManager::resolve(const std::string& remote_host, const std::uint16_t& port) {
		boost::asio::ip::udp::resolver resolver(_socket->io_context);
		std::string port_string = std::to_string(port);
		return *resolver.resolve(udp::v4(), remote_host, port_string.c_str()).begin();
	}

	Peer ClientNetworkManager::create_peer(const Endpoint& init_endpoint) {
		_peer = Peer(_socket, init_endpoint);
		return *_peer;
	}

	void ClientNetworkManager::start_receive() {
		_socket->socket.async_receive_from(
			boost::asio::buffer(_receive_buffer),
			_remote_endpoint,
			boost::bind(
				&ClientNetworkManager::handle_receive,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}

	void ClientNetworkManager::handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred) {
		if (_peer) {
			_peer->push_message(_receive_buffer);
		}

		start_receive();
	}
}
