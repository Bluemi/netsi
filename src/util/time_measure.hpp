#ifndef __NETSI_TIME_MEASURE_CLASS__
#define __NETSI_TIME_MEASURE_CLASS__

#include <iostream>
#include <chrono>

namespace netsi {
	class time_measure {
		public:
			void start() {
				t1 = std::chrono::high_resolution_clock::now();
			}

			void stop(const std::string& message) {
				const std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
				const std::chrono::duration<double, std::milli> time_span = t2 - t1;

				std::cout << message << time_span.count() << std::endl;
			}
		private:
			std::chrono::high_resolution_clock::time_point t1;
	};
}

#endif
