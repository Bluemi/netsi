#ifndef __NETSI_CYCLE_CLASS__
#define __NETSI_CYCLE_CLASS__

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace netsi {
	class Cycle {
		public:
			Cycle(boost::asio::io_context& io, const boost::posix_time::time_duration& interval);
			boost::posix_time::time_duration next();

		private:
			boost::posix_time::time_duration _interval;
			boost::asio::deadline_timer _timer;
	};
}

#endif
