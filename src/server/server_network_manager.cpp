#include "server_network_manager.hpp"

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

	bool server_network_manager::has_client_request() const {
		return !_client_requests.empty();
	}

	client_request server_network_manager::pop_client_request() {
		return _client_requests.pop();
	}

	peer server_network_manager::create_peer(const endpoint& remote_endpoint) {
		peer peer(_socket, remote_endpoint);
		_peers.insert({remote_endpoint, peer});
		return peer;
	}

	void server_network_manager::start_receive() {
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
		auto search_peer = _peers.find(_remote_endpoint);
		if (search_peer == _peers.end()) {
			_client_requests.push(client_request(
				std::vector<char>(_receive_buffer.cbegin(), _receive_buffer.cbegin()+bytes_transferred),
				_remote_endpoint
			));
		} else {
			search_peer->second.push_message(std::vector<char>(_receive_buffer.cbegin(), _receive_buffer.cbegin()+bytes_transferred));
		}

		start_receive();
	}
}
