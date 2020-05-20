#include "server_network_manager.hpp"

#include <iostream>

namespace netsi {
	ServerNetworkManager::ServerNetworkManager(const std::uint16_t port, const std::size_t buffer_size)
		: _socket(std::make_shared<Socket>(port)), _buffer_size(buffer_size), _receive_buffer(buffer_size)
	{
		start_receive();
		_socket->run();
	}

	ServerNetworkManager::~ServerNetworkManager() {
		_socket->stop();
	}

	bool ServerNetworkManager::has_client_request() const {
		return !_client_requests.empty();
	}

	ClientRequest ServerNetworkManager::pop_client_request() {
		return _client_requests.pop();
	}

	struct _CreatePeerVisitor {
		_CreatePeerVisitor(Peer& peer, const Endpoint& endpoint) : _peer(peer), _endpoint(endpoint) {}

		void operator()(Peer& p) {
		}

		void operator()(BlockingQueue<std::vector<char>>& message_queue) {
			while (!message_queue.empty()) {
				_peer.push_message(std::move(message_queue.pop()));
			}
		}

		Peer _peer;
		Endpoint _endpoint;
	};

	Peer ServerNetworkManager::create_peer(const Endpoint& remote_endpoint) {
		auto search_peer = _peers.find(remote_endpoint);
		if (search_peer != _peers.end()) {
			std::cerr << "ERROR: Trying to recreate peer for endpoint " << remote_endpoint << std::endl;
		} else {
			Peer peer(_socket, remote_endpoint);
			auto pending_peer_search = _pending_peers.find(remote_endpoint);
			if (pending_peer_search != _pending_peers.end()) {
				while (!pending_peer_search->second.empty()) {
					peer.push_message(std::move(pending_peer_search->second.pop()));
				}
			}
			_peers.insert({remote_endpoint, peer});
			return peer;
		}
	}

	void ServerNetworkManager::start_receive() {
		_socket->socket.async_receive_from(
			boost::asio::buffer(_receive_buffer),
			_remote_endpoint,
			boost::bind(
				&ServerNetworkManager::handle_receive,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}

	void ServerNetworkManager::handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred) {
		std::vector<char> buffer_copy(_receive_buffer.cbegin(), _receive_buffer.cbegin() + bytes_transferred);

		auto search_peer = _peers.find(_remote_endpoint);
		if (search_peer != _peers.end()) {
			search_peer->second.push_message(std::move(buffer_copy));
		} else {
			auto search_pending_peer = _pending_peers.find(_remote_endpoint);
			if (search_pending_peer != _pending_peers.end()) {
				search_pending_peer->second.push(std::move(buffer_copy));
			} else {
				_client_requests.push(ClientRequest(
					std::move(buffer_copy),
					_remote_endpoint
				));
				_pending_peers.insert(std::make_pair(_remote_endpoint, BlockingQueue<std::vector<char>>()));
			}
		}

		start_receive();
	}
}
