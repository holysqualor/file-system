#ifndef DISK_H_INCLUDED
#define DISK_H_INCLUDED

#include "Directory.h"
#include "File.h"

class Disk {
private:
    enum user_type {
        NONE,
        ROOT,
        USER
    };

    Directory *root, *current;
    user_type user;



    struct Workspace {
        Directory *directory;
        std::string target;
    };

    Workspace parse(std::string& path);
    static user_type login();

public:
    enum status {
        SUCCESS,
        TOO_FEW_ARGUMENTS,
        TOO_MANY_ARGUMENTS,
        BAD_PATH,
        ALREADY_EXISTS,
        NOT_FOUND,
        ACCESS_DENIED,
        INVALID_ARGUMENT,
        BAD_OPERATION
    };

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
    status pwd(std::vector<std::string> &args);
    status chmod(std::vector<std::string> &args);
    status cat(std::vector<std::string> &args);
    status echo(std::vector<std::string> &args);
    status mv(std::vector<std::string> &args);
    status logout(std::vector<std::string> &args);

    void banner() const;
};

#endif // DISK_H_INCLUDED
