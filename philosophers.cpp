#include "Philosophers.h"
#include "colours.h"

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <vector>

constexpr int MAX_DELAY{ 1000 };

std::random_device rd;
std::minstd_rand generator(rd());
std::uniform_int_distribution<int> distribution(0, MAX_DELAY);

const int PHILOSOPHERS_COUNT{ 5 };
const int MEALS_COUNT{ 3 };

Philosopher::Philosopher(int n, Fork& left, Fork& right, WaiterSemaphore& waiter)
    : number{ n }, mealsEaten{ 0 },
    leftHandFork{ left }, rightHandFork{ right }, waiterSemaphore{ waiter }
{ }

// DISPLAY HELPER FUNCTIONS ==========================
//
void eatDisplay(int nr, int mealsEaten, int mealTime, std::string colour)
{
    std::string info = "";
    info.append(colour).append("Philosopher_").append(std::to_string(nr)).append(" eats ")
        .append(std::to_string(mealsEaten))
        .append(". meal for ")
        .append(std::to_string(mealTime))
        .append(" ms\n").append(Colour::reset());

    std::cout << info;
}

void finishedMealDisplay(int nr, int mealsEaten, std::string colour)
{
    std::string info = "";
    info.append(colour).append("Philosopher_").append(std::to_string(nr))
        .append(" finished eating ").append(std::to_string(mealsEaten))
        .append(". meal\n").append(Colour::reset());

    std::cout << info;
}


void displayForkLift(int philosopherNum, const Fork& f)
{
    std::cout << "\tPhilosopher_" + std::to_string(philosopherNum) + " lifts fork_" + std::to_string(f.getNr()) + "\n";
}

void displayForkPutoff(int philosopherNum, const Fork& f)
{
    std::cout << "\tPhilosopher_ " + std::to_string(philosopherNum) + " puts off fork_" + std::to_string(f.getNr()) + "\n";
}

// ==========================

// Philosopher eats a few meals, each time:
// lifting left hand fork first, assuming that it is available
// (if not, he waits for other philosopher to put it off),
// then lifting right hand fork in the same way.
// He then eats (thread is sleeping), which takes some time (based on a given mealTime),
// and puts off forks, left one first again.
//
// Waiter (semaphore) makes sure if the are maximally 4 philosophers eating at the same time
// to avoid deadlock.
void Philosopher::operator()()
{
    std::string colour{ Colour::next() };

    while (mealsEaten < MEALS_COUNT)
    {
        int mealTime = distribution(generator);

        waiterSemaphore.wait();

        displayForkLift(this->number, leftHandFork);
        leftHandFork.lift();
        displayForkLift(this->number, rightHandFork);
        rightHandFork.lift();

        ++(this->mealsEaten);
        eatDisplay(this->number, this->mealsEaten, mealTime, colour);
        std::this_thread::sleep_for(std::chrono::milliseconds(mealTime));

        displayForkPutoff(this->number, leftHandFork);
        leftHandFork.putOff();
        displayForkPutoff(this->number, rightHandFork);
        rightHandFork.putOff();

        waiterSemaphore.signalize();

        finishedMealDisplay(this->number, this->mealsEaten, colour);
    }

    std::string info = "";
    info.append("Philosopher ").append(std::to_string(this->number))
        .append(" is full\n");
    std::cout << info;
}

void Fork::lift()
{
    semaphore.wait();
    lifted = true;
}

void Fork::putOff()
{
    semaphore.signalize();
    lifted = false;
}
