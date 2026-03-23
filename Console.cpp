#include "Console.h"

Disk Console::disk;
std::vector<std::string> Console::args;
const char *Console::error[] = {
    "",
    "Too few arguments",
    "Too many arguments",
    "Bad path",
    "An object with this name already exists",
    "Object not found",
    "Access denied",
    "Invalid argument",
    "The operation cannot be performed"
};

bool Console::run() {
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
        Disk::status status = Disk::SUCCESS;
        if(cmd == "poweroff") {
            if(!args.empty())
                status = Disk::TOO_MANY_ARGUMENTS;
            else return false;
        }
        else if(cmd == "touch")
            status = disk.touch(args);
        else if(cmd == "mkdir")
            status = disk.mkdir(args);
        else if(cmd == "rm")
            status = disk.rm(args);
        else if(cmd == "cd")
            status = disk.cd(args);
        else if(cmd == "ls")
            status = disk.ls(args);
        else if(cmd == "pwd")
            status = disk.pwd(args);
        else if(cmd == "chmod")
            status = disk.chmod(args);
        else if(cmd == "cat")
            status = disk.cat(args);
        else if(cmd == "echo")
            status = disk.echo(args);
        else if(cmd == "mv")
            status = disk.mv(args);
        else if(cmd == "logout")
            status = disk.logout(args);
        else {
            std::cout << "bash: " << cmd << ": Unknown command" << std::endl;
            break;
        }
        if(status) {
            std::cout << "bash: " << cmd << ": " << error[status] << std::endl;
            break;
        }
    }

    return true;
}
