#ifndef __NETSI_TIME_MEASURE_CLASS__
#define __NETSI_TIME_MEASURE_CLASS__

#include <chrono>
#include <string>

namespace netsi {
	class TimeMeasure {
		public:
			void start();
			void stop(const std::string& message);
		private:
			std::chrono::high_resolution_clock::time_point t1;
	};
}

#endif
