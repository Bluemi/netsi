#ifndef __NETSI_PEER_CLASS__
#define __NETSI_PEER_CLASS__

#include <boost/asio.hpp>

#include "socket.hpp"
#include "message_types.hpp"

namespace netsi {
	class peer {
		public:
			peer(socket_ptr socket, boost::asio::ip::udp::endpoint remote_endpoint);

			template<typename T>
			void send(const T& msg) {
				std::shared_ptr<std::vector<char>> buffer = std::make_shared<std::vector<char>>();
				buffer->push_back(MessageType::DATA);
				buffer->insert(buffer->end(), msg.cbegin(), msg.cend());

				_socket->socket.async_send_to(
					boost::asio::buffer(*buffer),
					_remote_endpoint,
					boost::bind(
						&peer::handle_send,
						this,
						buffer,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);
			}

			bool has_message() const;
			std::vector<char> pop_message();
		private:
			void handle_send(const std::shared_ptr<std::vector<char>> message, const boost::system::error_code& error_code, std::size_t bytes_transferred);

			std::uint32_t _id;
			socket_ptr _socket;
			boost::asio::ip::udp::endpoint _remote_endpoint;
	};
}

#endif
