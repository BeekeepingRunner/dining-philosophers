#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class Semaphore
{
protected:
	int resourceCounter;

	std::mutex counterMutex;
	std::condition_variable counterGreaterThanZero;

public:
	Semaphore(int _count = 1);

	void signalize();
	void wait();

	int getResourceCounter() const;
};

class WaiterSemaphore : public Semaphore
{
public:
	WaiterSemaphore(int _count = 4);
};

#endif