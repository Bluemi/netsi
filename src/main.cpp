#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>

#include "cycle.hpp"
#include "blocking_queue.hpp"

using boost::posix_time::milliseconds;

class producer {
	public:
		producer(boost::asio::io_context* io, blocking_queue<int>* q) : _io(io), _q(q) {}
		void operator()() {
			int i = 10;
			
			for (cycle c(*_io, milliseconds(200)); i >= 0; c.next(), i--) {
				_q->push(i);
				std::cout << "pushed " << i << std::endl;
			}
		}
	private:
		boost::asio::io_context* _io;
		blocking_queue<int>* _q;
};

class consumer {
	public:
		consumer(boost::asio::io_context* io, blocking_queue<int>* q) : _io(io), _q(q) {}

		void operator()() {
			int i = 1;
			for (cycle c(*_io, milliseconds(400)); i; c.next()) {
				i = _q->pop();
				std::cout << "popped " << i << std::endl;
			}
		}
	private:
		boost::asio::io_context* _io;
		blocking_queue<int>* _q;
};

int main() {
	blocking_queue<int> q;
	boost::asio::io_context io;

	consumer c(&io, &q);
	boost::thread consumer_thread(c);

	producer p(&io, &q);
	boost::thread producer_thread(p);

	consumer_thread.join();
	producer_thread.join();
	return 0;
}
