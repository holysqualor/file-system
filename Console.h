#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include "Disk.h"

#include <iostream>
#include <string>
#include <vector>

class Console {
private:
    struct Command {
        const char *name;
        Disk::status (Disk::*run)(std::vector<std::string> &args);
    };

    static const char *error[];
    static std::vector<std::string> args;
    static const Command commands[];

public:
    static bool process(Disk &disk);
};

#endif // CONSOLE_H_INCLUDED
