#ifndef DISK_H_INCLUDED
#define DISK_H_INCLUDED

#include "Directory.h"
#include "File.h"

class Disk {
private:
    Directory *root, *current;

public:
    Disk();
    ~Disk();

    void touch(const std::string& name);
    void mkdir(const std::string& name);
    void cd(const std::string& name);
    void rename(const std::string& oldName, const std::string& newName);
    void echo(const std::string& name, const std::string& content);
    void cat(const std::string& name);
    void ls() const;
};

#endif // DISK_H_INCLUDED
