#ifndef __NETSI_SERVER_NETWORK_MANAGER_CLASS__
#define __NETSI_SERVER_NETWORK_MANAGER_CLASS__

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "server_peer_builder.hpp"
#include "../util/endpoint.hpp"

namespace netsi {
	/**
	 * Wraps a server peer builder and starts it in a new thread.
	 * Runs and handles the io_context.
	 */
	template<std::size_t BUFFER_SIZE>
	class server_network_manager
	{
		public:
			server_network_manager() : _io_context(), _run_thread(), _server_peer_builder() {}

			~server_network_manager() {
				join();
			}

			blocking_queue<endpoint>& get_connecting_endpoints() {
				return _server_peer_builder->get_connecting_endpoints();
			}

			boost::asio::io_context& get_context() {
				return _io_context;
			}

			void run(unsigned short port) {
				_server_peer_builder = std::make_unique<server_peer_builder<BUFFER_SIZE>>(_io_context, port);
				_run_thread = boost::thread(std::ref(*this));
			}

			void stop() {
				_io_context.stop();
				join();
			}

			void join() {
				if (_run_thread) {
					_run_thread->join();
					_run_thread = {};
				}
			}

			void operator()() {
				_server_peer_builder->init();
				_io_context.run();
			}

			std::shared_ptr<peer<BUFFER_SIZE>> endpoint_to_peer(const endpoint& remote_endpoint) {
				std::shared_ptr<peer<BUFFER_SIZE>> new_peer = std::shared_ptr<peer<BUFFER_SIZE>>(new peer<BUFFER_SIZE>(_io_context));
				new_peer->connect(remote_endpoint);
				new_peer->init();
				new_peer->send_kick_off();
				return new_peer;
			}
		private:
			boost::asio::io_context _io_context;
			std::optional<boost::thread> _run_thread;
			std::unique_ptr<server_peer_builder<BUFFER_SIZE>> _server_peer_builder;
	};
}

#endif
