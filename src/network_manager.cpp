#ifndef __NETSI_NETWORK_MANAGER_CLASS__
#define __NETSI_NETWORK_MANAGER_CLASS__

#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "peer.hpp"
#include "blocking_queue.hpp"

using boost::asio::ip::udp;

class server_peer_builder {
	public:
		server_peer_builder(boost::asio::io_context& io_context, unsigned short port)
			: _io_context(&io_context), _socket(std::make_shared<udp::socket>(io_context, udp::endpoint(udp::v4(), port))) {}

		void init() {
			std::cout << "listening on " << _socket->local_endpoint() << std::endl;
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

		const std::vector<std::shared_ptr<peer>>& get_peers() const {
			return _peers;
		}

	private:
		void handle_receive(const boost::system::error_code& error_code, std::size_t bytes_transferred) {
			if (error_code) {
				std::cout << "error creating connection" << std::endl;
			} else {
				std::shared_ptr<peer> p = std::make_shared<peer>(*_io_context, _remote_endpoint);
				p->contact_remote();
				p->init();
				_peers.push_back(p);
			}

			start_receive();
		}

		boost::asio::io_context* _io_context;
		std::shared_ptr<udp::socket> _socket;
		udp::endpoint _remote_endpoint;
		boost::array<char, 256> _recv_buf;
		std::vector<std::shared_ptr<peer>> _peers;
};

int main() {
	boost::asio::io_context io_context;

	server_peer_builder spb(io_context, 1350);
	spb.init();
	io_context.run();
	return 0;
}

#endif
