#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include "Disk.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

class Console {
private:
    static Disk disk;
    static std::vector<std::string> args;
    static const char *error[];

public:
    static bool run();
};

#endif // CONSOLE_H_INCLUDED
