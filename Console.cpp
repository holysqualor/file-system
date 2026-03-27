#include "Console.h"

#include <sstream>

std::vector<std::string> Console::args;
const char *Console::error[] = {
    "",
    "Unknown command",
    "Missing arguments",
    "Too many arguments",
    "Bad path",
    "An object with this name already exists",
    "Object not found",
    "Access denied",
    "Invalid argument",
    "The operation cannot be performed",
    "File required",
    "Directory required"
};
const Console::Command Console::commands[] = {
    {"touch", &Disk::touch},
    {"mkdir", &Disk::mkdir},
    {"rm", &Disk::rm},
    {"cd", &Disk::cd},
    {"ls", &Disk::ls},
    {"chmod", &Disk::chmod},
    {"cat", &Disk::cat},
    {"echo", &Disk::echo},
    {"mv", &Disk::mv},
    {"cp", &Disk::cp},
    {"logout", &Disk::logout}
};

bool Console::process(Disk &disk) {
    disk.banner();
    std::string input, arg, cmd;
    std::getline(std::cin, input);
    if(input.empty())
        return true;
    std::stringstream stream(input);
    while(!stream.eof()) {
        std::getline(stream, cmd, ' ');
        args.clear();
        while(std::getline(stream, arg, ' ') && arg != "&&")
            args.push_back(arg);
        Disk::status status = Disk::UNKNOWN_COMMAND;
        if(cmd == "poweroff") {
            if(!args.empty())
                status = Disk::TOO_MANY_ARGUMENTS;
            else return false;
        } else {
            for(const Command &command : commands) {
                if(cmd == command.name) {
                    status = (disk.*command.run)(args);
                    break;
                }
            }
        }
        if(status) {
            std::cout << "bash: " << cmd << ": " << error[status] << std::endl;
            break;
        }
    }
    return true;
}
