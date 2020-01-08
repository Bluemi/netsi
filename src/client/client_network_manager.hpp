#ifndef __NETSI_CLIENT_NETWORK_MANAGER_CLASS__
#define __NETSI_CLIENT_NETWORK_MANAGER_CLASS__

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../peer/peer.hpp"
#include "../util/endpoint.hpp"

namespace netsi {
	using boost::asio::ip::udp;

	template<std::size_t BUFFER_SIZE>
	class client_network_manager {
		public:
			client_network_manager() : _io_context(), _run_thread() {}
			~client_network_manager() {
				join();
			}

			endpoint resolve(const std::string& remote_host, const std::string& port) {
				udp::resolver resolver(_io_context);
				return *resolver.resolve(udp::v4(), remote_host, port).begin();
			}

			std::shared_ptr<peer<BUFFER_SIZE>> create_peer(const endpoint& init_endpoint) {
				std::shared_ptr<peer<BUFFER_SIZE>> p = std::make_shared<peer<BUFFER_SIZE>>(_io_context);
				p->send_kick_off(init_endpoint);

				endpoint data_endpoint = p->listen_for_kick_off();
				p->connect(data_endpoint);
				p->init();

				return p;
			}

			void run() {
				_run_thread = boost::thread(std::ref(*this));
			}

			void operator()() {
				_io_context.run();
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

			boost::asio::io_context& get_context() {
				return _io_context;
			}
		private:
			boost::asio::io_context _io_context;
			std::optional<boost::thread> _run_thread;
	};
}

#endif
