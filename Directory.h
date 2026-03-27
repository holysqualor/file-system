#ifndef DIRECTORY_H_INCLUDED
#define DIRECTORY_H_INCLUDED

#include <iostream>
#include <set>
#include <cstdint>
#include "FSObject.h"
#include "File.h"

class Directory : public FSObject {
private:
    std::set<FSObject*> child;
    Directory *parent;



public:
    Directory(const std::string& name, uint8_t mode);
    Directory();
    Directory(const std::string& name);
    Directory(const Directory& other);
    ~Directory();

    FSObject *get(const std::string& name);
    Directory *getDirectory(const std::string& name);
    File *getFile(const std::string& name);

    FSObject *clone() override;
    bool isDirectory() const override;
    bool isFile() const override;
    void save(std::ofstream &dest) override;

    void add(FSObject *obj);
    void erase(FSObject *object);
    void show();
    bool has(Directory *other);
};

#endif // DIRECTORY_H_INCLUDED
