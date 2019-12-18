#ifndef __BLOCKING_QUEUE_CLASS__
#define __BLOCKING_QUEUE_CLASS__

#include <mutex>
#include <condition_variable>
#include <deque>

/**
 * Taken from https://stackoverflow.com/questions/12805041/c-equivalent-to-javas-blockingqueue
 */
template <typename T>
class blocking_queue
{
	public:
		void push(T const& value) {
			{
				std::unique_lock<std::mutex> lock(this->d_mutex);
				d_queue.push_front(value);
			}
			this->d_condition.notify_one();
		}

		T pop() {
			std::unique_lock<std::mutex> lock(this->d_mutex);
			this->d_condition.wait(lock, [=]{ return !this->d_queue.empty(); });
			T rc(std::move(this->d_queue.back()));
			this->d_queue.pop_back();
			return rc;
		}

		bool empty() const {
			return d_queue.empty();
		}
	private:
		std::mutex              d_mutex;
		std::condition_variable d_condition;
		std::deque<T>           d_queue;
};
#endif
