#include "colours.h"

unsigned short Colour::counter = 0;
std::mutex Colour::mtx;

const std::string Colour::RESET = "\033[0m";

const std::string Colour::COLOUR[] = {
    "\033[0;31m", //01
    "\033[1;32m", //02
    "\033[1;33m", //03
    "\033[1;34m", //04
    "\033[0;35m", //05
    "\033[0;43m", //06
    "\033[0;44m", //07
    "\033[0;45m", //08
    "\033[0;46m", //09
    "\033[0;40m", //10
    "\033[0;41m", //11
    "\033[0;47m", //12
};

std::string Colour::reset() {
    return RESET;
}

std::string Colour::next() {
    mtx.lock();
    std::string colour = COLOUR[counter];
    counter = ++counter < COLOUR_COUNT ? counter : 0;
    mtx.unlock();
    return colour;
}

std::string Colour::test() {
    std::string result = "\n---Colours test---\n";
    for (int i = 1; i <= Colour::COLOUR_COUNT; i++) {
        result += Colour::next() + "Colour " + std::to_string(i) + "\n";
    }
    result += Colour::reset() + "---Test end---\n";

    return result;
}