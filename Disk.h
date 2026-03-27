#ifndef DISK_H_INCLUDED
#define DISK_H_INCLUDED

#include "Directory.h"
#include "File.h"
#include <vector>

class Disk {
private:
    enum user_type {
        NONE,
        ROOT,
        USER
    };
    Directory *root, *current;
    user_type user;
    std::string filename;

    struct Workspace {
        Directory *directory;
        std::string target;

        bool empty() {
            return directory == NULL || target.empty();
        }
    };
    Workspace parse(std::string& path);
    static user_type login();
    static const uint32_t VDI;

public:
    enum status {
        SUCCESS,
        UNKNOWN_COMMAND,
        TOO_FEW_ARGUMENTS,
        TOO_MANY_ARGUMENTS,
        BAD_PATH,
        ALREADY_EXISTS,
        NOT_FOUND,
        ACCESS_DENIED,
        INVALID_ARGUMENT,
        BAD_OPERATION,
        NOT_FILE,
        NOT_DIRECTORY
    };

    Disk(const std::string &filename);
    Disk();
    Disk(const Disk& other);
    Disk(Disk&& other);
    Disk& operator=(const Disk& other);
    ~Disk();

    status touch(std::vector<std::string> &args);
    status mkdir(std::vector<std::string> &args);
    status rm(std::vector<std::string> &args);
    status cd(std::vector<std::string> &args);
    status ls(std::vector<std::string> &args);
    status chmod(std::vector<std::string> &args);
    status cat(std::vector<std::string> &args);
    status echo(std::vector<std::string> &args);
    status mv(std::vector<std::string> &args);
    status cp(std::vector<std::string> &args);
    status logout(std::vector<std::string> &args);

    void banner() const;
};

#endif // DISK_H_INCLUDED
