#ifndef DIRECTORY_H_INCLUDED
#define DIRECTORY_H_INCLUDED

#include <iostream>
#include <vector>
#include <cstdint>
#include "FSObject.h"
#include "File.h"

class Directory : public FSObject {
private:
    std::vector<FSObject*> child;
    FSObject *parent;

    Directory(const std::string& name, Directory *parent);
    FSObject *find(const std::string& name);

public:
    Directory();
    ~Directory();


    File *getFile(const std::string& name);
    Directory *getDirectory(const std::string& name);
    void addFile(const std::string& name);
    void addDirectory(const std::string& name);
    void rm(const std::string& name);
    void ls();
    void rename(const std::string& oldName, const std::string& newName);
};

#endif // DIRECTORY_H_INCLUDED
