#include "colours.h"
#include "Philosophers.h"
#include "Semaphore.h"

#include <iostream>
#include <vector>

int main()
{
    std::vector<std::thread> philosophers;

    Fork w0(0);
    Fork w1(1);
    Fork w2(2);
    Fork w3(3);
    Fork w4(4);

    WaiterSemaphore waiter(4);

    for (int i = 0; i < 1; ++i)
    {
        // philosophers eat
        philosophers.emplace_back(Philosopher(0, w0, w1, waiter));
        philosophers.emplace_back(Philosopher(1, w1, w2, waiter));
        philosophers.emplace_back(Philosopher(2, w2, w3, waiter));
        philosophers.emplace_back(Philosopher(3, w3, w4, waiter));
        philosophers.emplace_back(Philosopher(4, w4, w0, waiter));

        for (std::thread& f : philosophers)
            f.join();

        philosophers.pop_back();
        philosophers.pop_back();
        philosophers.pop_back();
        philosophers.pop_back();
        philosophers.pop_back();

        std::cout << "\n\n";
    }

    std::cout << "All philosophers are full!\n";
    
	return 0;
}