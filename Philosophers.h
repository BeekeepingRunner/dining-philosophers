#ifndef FILOZOFOWIE_H
#define FILOZOFOWIE_H

#include "Semaphore.h"
#include <random>

class Fork
{
private:
	int nr;
	Semaphore semaphore;

	bool lifted = false;

public:
	Fork(int n) : nr{ n }
	{}

	void lift();
	void putOff();
	int getNr() const { return nr; }
};

class Philosopher
{
private:
	int number;
	int mealsEaten;

	Fork& leftHandFork;
	Fork& rightHandFork;
	WaiterSemaphore& waiterSemaphore;

public:
	Philosopher(int n, Fork& left, Fork& right, WaiterSemaphore& waiter);

	void operator()();
};

#endif