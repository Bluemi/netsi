#include "time_measure.hpp"

#include <iostream>

namespace netsi {
	void TimeMeasure::start() {
		t1 = std::chrono::high_resolution_clock::now();
	}

	void TimeMeasure::stop(const std::string& message) {
		const std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<double, std::milli> time_span = t2 - t1;

		std::cout << message << time_span.count() << std::endl;
	}
}
