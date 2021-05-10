#include "Semaphore.h"

Semaphore::Semaphore(int _count)
	: resourceCounter{ _count }
{
}

int Semaphore::getResourceCounter() const
{
	return resourceCounter;
}

void Semaphore::signalize()
{
	std::unique_lock<std::mutex> counterBlockade(counterMutex);

	++resourceCounter;
	counterGreaterThanZero.notify_one();
}

void Semaphore::wait()
{
	std::unique_lock<std::mutex> counterBlockade(counterMutex);

	while (resourceCounter == 0)
		counterGreaterThanZero.wait(counterBlockade);

	--resourceCounter;
}

WaiterSemaphore::WaiterSemaphore(int _count)
	: Semaphore(_count)
{
}