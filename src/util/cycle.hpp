#ifndef __NETSI_CYCLE_CLASS__
#define __NETSI_CYCLE_CLASS__

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace netsi {
	class cycle
	{
		public:
			cycle(boost::asio::io_context& io, const boost::posix_time::time_duration& interval);
			boost::posix_time::time_duration next();

		private:
			boost::posix_time::time_duration _interval;
			boost::asio::deadline_timer _timer;
	};

	cycle::cycle(boost::asio::io_context& io, const boost::posix_time::time_duration& interval) : _interval(interval), _timer(io, interval) {}

	boost::posix_time::time_duration cycle::next() {
		_timer.wait();
		_timer.expires_at(_timer.expires_at() + _interval);

		return boost::posix_time::seconds(0);
	}
}

#endif
