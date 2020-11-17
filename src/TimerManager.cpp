#include "TimerManager.h"

#include <memory>
#include <queue>
#include <random>

using namespace std::literals;
namespace fibers = boost::fibers;

void timer_loop(fibers::unbuffered_channel<RepeatingTimerRequest>& rx);
template <class RandomGenerator>
std::chrono::steady_clock::time_point calculateNextFire(std::chrono::milliseconds min, std::chrono::milliseconds max, RandomGenerator& generator);

TimerManager::TimerManager()
	: tx{}
	, timer_loop_fiber{timer_loop, std::ref(tx)}
{

}

TimerManager::~TimerManager() {
	tx.close();
	timer_loop_fiber.join();
}

void TimerManager::addTimer(TimerRequest request) {
	RepeatingTimerRequest repeatingRequest = {
		.callback = [request]() { request.callback(); return false; },
		.min = request.min,
		.max = request.max,
		.ignoreMinFirstTime = false,
	};

	addTimer(repeatingRequest);
}

void TimerManager::addTimer(RepeatingTimerRequest request) {
	tx.push(request);
}

struct CompleteRepeatingTimerRequest {
	std::function<bool()> callback;
	std::chrono::milliseconds min;
	std::chrono::milliseconds max;
	std::chrono::steady_clock::time_point nextFire;
};

void timer_loop(fibers::unbuffered_channel<RepeatingTimerRequest>& rx) {
	auto cmp = [](CompleteRepeatingTimerRequest a, CompleteRepeatingTimerRequest b) {
		return a.nextFire > b.nextFire;
	};
	using InnerContainer = std::vector<CompleteRepeatingTimerRequest>;
	std::priority_queue<CompleteRepeatingTimerRequest, InnerContainer, decltype(cmp)> timers{cmp};
	std::mt19937 generator{std::random_device{}()};

	while(true) {
		RepeatingTimerRequest addTimer;
		fibers::channel_op_status status;
		if(timers.size()) {
			status = rx.pop_wait_until(addTimer, timers.top().nextFire);
		} else {
			status = rx.pop(addTimer);
		}

		if(status == fibers::channel_op_status::success) {
			CompleteRepeatingTimerRequest completeTimer = {
				.callback = addTimer.callback,
				.min = addTimer.min,
				.max = addTimer.max,
				.nextFire = calculateNextFire(addTimer.ignoreMinFirstTime ? 0s : addTimer.min, addTimer.max, generator),
			};
			timers.push(completeTimer);
		} else if (status == fibers::channel_op_status::timeout) {
			auto now = std::chrono::steady_clock::now();
			while(timers.size() && timers.top().nextFire < now) {
				auto next = timers.top();
				timers.pop();
				if(next.callback()) {
					next.nextFire = calculateNextFire(addTimer.min, addTimer.max, generator);
					timers.push(next);
				}
			}
		} else {
			break;
		}
	}
}

template <class RandomGenerator>
std::chrono::steady_clock::time_point calculateNextFire(std::chrono::milliseconds min, std::chrono::milliseconds max, RandomGenerator& generator) {
	auto now = std::chrono::steady_clock::now();
	auto min_point = now + min;
	auto max_point = now + max;
	
	using rep_type = std::chrono::steady_clock::time_point::duration::rep;
	
	std::uniform_int_distribution<rep_type> distribution{min_point.time_since_epoch().count(), max_point.time_since_epoch().count()};
	std::chrono::steady_clock::time_point::duration duration{distribution(generator)};
	return std::chrono::steady_clock::time_point{duration};
}
