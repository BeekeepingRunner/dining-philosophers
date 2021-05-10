#ifndef COLOURS_H
#define COLOURS_H

#include <string>
#include <mutex>

// This class comes from my university teacher's materials

class Colour {
private:
    static unsigned short counter;
    static std::mutex mtx;
    static const std::string RESET;
    static const std::string COLOUR[];
public:
    static const unsigned short COLOUR_COUNT = 12;
    static std::string reset();
    static std::string next();
    static std::string test();
};

#endif
