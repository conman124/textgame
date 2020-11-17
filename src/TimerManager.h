#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <boost/fiber/fiber.hpp>
#include <boost/fiber/unbuffered_channel.hpp>

#include <chrono>
#include <functional>

struct TimerRequest {
	std::function<void()> callback;
	std::chrono::milliseconds min;
	std::chrono::milliseconds max;
};

struct RepeatingTimerRequest {
	std::function<bool()> callback;
	std::chrono::milliseconds min;
	std::chrono::milliseconds max;
	bool ignoreMinFirstTime;
};

class TimerManager {
	public:
		TimerManager();
		~TimerManager();
		TimerManager(const TimerManager&) = delete;
		TimerManager& operator=(const TimerManager&) = delete;

		void addTimer(TimerRequest request);
		void addTimer(RepeatingTimerRequest request);
	private:
		boost::fibers::unbuffered_channel<RepeatingTimerRequest> tx;
		boost::fibers::fiber timer_loop_fiber;
};

#endif