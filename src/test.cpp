#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class printer {
	public:
		printer(boost::asio::io_context& io) : _timer(io, boost::posix_time::seconds(1)), _count(0) {
			_timer.async_wait(boost::bind(&printer::print, this));
		}

		void print() {
			if (_count < 5) {
				std::cout << _count << std::endl;
				_count++;

				_timer.expires_at(_timer.expires_at() + boost::posix_time::seconds(1));
				_timer.async_wait(boost::bind(&printer::print, this));
			}
		}
	private:
		boost::asio::deadline_timer _timer;
		int _count;
};

int main() {
	boost::asio::io_context io;
	printer p(io);
	io.run();

	return 0;
}
