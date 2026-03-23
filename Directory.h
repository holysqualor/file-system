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
    Directory *parent;

    Directory(const std::string& name, Directory *parent);

public:
    Directory();
    Directory(const Directory& other);
    ~Directory();

    FSObject *find(const std::string& name);

    FSObject *clone() override;
    bool isDirectory() const override;
    bool isFile() const override;

    std::string getType() const;

    File *getFile(const std::string& name);
    Directory *getDirectory(const std::string& name);
    void addFile(const std::string& name);
    void addFile(const std::string& name, const std::string& content);
    void addDirectory(const std::string& name);
    void push(FSObject *obj);
    FSObject *pop(const std::string& name);
    void rm(const std::string& name);
    void ls();
    void pwd();
    bool isindp(Directory *other);
};

#endif // DIRECTORY_H_INCLUDED
