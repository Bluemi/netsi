#ifndef __NETSI_PEER_CLASS__
#define __NETSI_PEER_CLASS__

#include <memory>
#include <boost/asio.hpp>

#include "endpoint.hpp"
#include "socket.hpp"
#include "../util/blocking_queue.hpp"

namespace netsi {
	class Peer {
		public:
			Peer() = default;
			Peer(SocketPtr socket, Endpoint remote_endpoint);

			template<typename T>
			void send(const T& msg) {
				std::shared_ptr<std::vector<char>> buffer = std::make_shared<std::vector<char>>(msg.cbegin(), msg.cend());

				_socket->socket.async_send_to(
					boost::asio::buffer(*buffer),
					_remote_endpoint,
					boost::bind(
						&Peer::handle_send,
						this,
						buffer,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);
			}

			void send(const char* msg) {
				send(std::string(msg));
			}

			bool has_message() const;
			std::vector<char> pop_message();
			void push_message(const std::vector<char>& message);
		private:
			void handle_send(const std::shared_ptr<std::vector<char>> message, const boost::system::error_code& error_code, std::size_t bytes_transferred);

			std::uint32_t _id;
			SocketPtr _socket;
			Endpoint _remote_endpoint;
			std::shared_ptr<BlockingQueue<std::vector<char>>> _messages;
	};
}

#endif
