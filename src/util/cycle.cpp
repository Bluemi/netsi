#include "cycle.hpp"

namespace netsi {
	Cycle::Cycle(boost::asio::io_context& io, const boost::posix_time::time_duration& interval) : _interval(interval), _timer(io, interval) {}

	boost::posix_time::time_duration Cycle::next() {
		_timer.wait();
		_timer.expires_at(_timer.expires_at() + _interval);

		return boost::posix_time::seconds(0);
	}
}
