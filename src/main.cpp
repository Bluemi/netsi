#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "cycle.hpp"

int main() {
	boost::asio::io_context io;

	boost::posix_time::time_duration t;

	unsigned int running = 5;

	for (cycle c(io, boost::posix_time::milliseconds(100)); running; t = c.next()) {
		std::cout << "running: " << running << std::endl;
		running--;
	}

	return 0;
}
