#ifndef __NETSI_NETWORK_MANAGER_CLASS__
#define __NETSI_NETWORK_MANAGER_CLASS__

#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "../peer/peer.hpp"
#include "../util/blocking_queue.hpp"
#include "../util/endpoint.hpp"

namespace netsi {
	using boost::asio::ip::udp;

	template<std::size_t BUFFER_SIZE>
	class server_peer_builder {
		public:
			server_peer_builder(boost::asio::io_context& io_context, unsigned short port)
				: _io_context(&io_context), _socket(std::make_shared<udp::socket>(io_context, endpoint(udp::v4(), port))) {}

			void init() {
				start_receive();
			}

			void start_receive() {
				_socket->async_receive_from(
					boost::asio::buffer(_recv_buf),
					_remote_endpoint,
					boost::bind(
						&server_peer_builder::handle_receive,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);
			}

			const std::vector<std::shared_ptr<peer<BUFFER_SIZE>>>& get_peers() const {
				return _peers;
			}

			blocking_queue<endpoint>& get_connecting_endpoints() {
				return _connecting_endpoints;
			}

		private:
			void handle_receive(const boost::system::error_code& error_code, std::size_t /*bytes_transferred*/) {
				if (error_code) {
					std::cerr << "error creating connection" << std::endl;
				} else {
					_connecting_endpoints.push(_remote_endpoint);
				}

				start_receive();
			}

			boost::asio::io_context* _io_context;
			std::shared_ptr<udp::socket> _socket;
			endpoint _remote_endpoint;
			boost::array<char, 256> _recv_buf;
			blocking_queue<endpoint> _connecting_endpoints;
			std::vector<std::shared_ptr<peer<BUFFER_SIZE>>> _peers;
	};
}

#endif
