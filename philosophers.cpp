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
/*
void wyswietlZeJe(std::string k, std::string r, int nr, int zjedzonychPosilkow, int czasPosilku)
{
    std::string info = "";
    info.append(k)
        .append("Filozof ").append(std::to_string(nr)).append(" je ")
        .append(std::to_string(zjedzonychPosilkow))
        .append(" posi³ek przez najbli¿sze ")
        .append(std::to_string(czasPosilku)).append(" ms\n").append(r);

    std::cout << info;
}

void wyswietlZeSkonczyl(std::string k, std::string r, int nr, int zjedzonychPosilkow)
{
    std::string info = "";
    info.append(k).append("Filozof ").append(std::to_string(nr))
        .append(" skoñczy³ jeœæ ").append(std::to_string(zjedzonychPosilkow))
        .append(" posi³ek\n").append(r);

    std::cout << info;
    info = "";
}
*/
void eatDisplay(int nr, int mealsEaten, int mealTime, std::string colour)
{
    std::string info = "";
    info.append(colour).append("Philosopher ").append(std::to_string(nr)).append(" eats ")
        .append(std::to_string(mealsEaten))
        .append(". meal for ")
        .append(std::to_string(mealTime))
        .append(" ms\n").append(Colour::reset());

    std::cout << info;
}

void finishedMealDisplay(int nr, int mealsEaten, std::string colour)
{
    std::string info = "";
    info.append(colour).append("Philosopher ").append(std::to_string(nr))
        .append(" finished eating ").append(std::to_string(mealsEaten))
        .append(". meal\n").append(Colour::reset());

    std::cout << info;
}

/*
void wyswietlZePodnosi(int nrFilozofa, const Widelec& w)
{
    std::cout << "Filozof " + std::to_string(nrFilozofa) + " podnosi w" + std::to_string(w.podajNr()) + "\n";
}

void wyswietlZeOdklada(int nrFilozofa, const Widelec& w)
{
    std::cout << "Filozof " + std::to_string(nrFilozofa) + " odk³ada w" + std::to_string(w.podajNr()) + "\n";
}
*/
// ==========================

void Philosopher::operator()()
{
    std::string colour{ Colour::next() };

    while (mealsEaten < MEALS_COUNT)
    {
        int mealTime = distribution(generator);

        // wyswietlZePodnosi(this->numer, lewyWidelec);
        waiterSemaphore.wait();
        leftHandFork.lift();
        // wyswietlZePodnosi(this->numer, prawyWidelec);
        rightHandFork.lift();

        ++(this->mealsEaten);
        eatDisplay(this->number, this->mealsEaten, mealTime, colour);
        // wyswietlZeJe(k, r, this->numer, zjedzonychPosilkow, czasPosilku);
        std::this_thread::sleep_for(std::chrono::milliseconds(mealTime));

        // wyswietlZeOdklada(this->numer, lewyWidelec);
        leftHandFork.putOff();
        // wyswietlZeOdklada(this->numer, prawyWidelec);
        rightHandFork.putOff();
        waiterSemaphore.signalize();

        //wyswietlZeSkonczyl(k, r, this->numer, zjedzonychPosilkow);
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
