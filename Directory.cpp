#include "Directory.h"

Directory::Directory(const std::string& name, uint8_t mode) : FSObject(name, mode), parent{NULL} {}

Directory::Directory(const std::string& name) : Directory(name, FSObject::READ | FSObject::WRITE | FSObject::EXECUTE) {}

Directory::Directory() : Directory("/") {}

Directory::Directory(const Directory& other) : FSObject(other), parent{NULL} {
    for(FSObject *object : other.child)
        add(object->clone());
}

Directory::~Directory() {
    for(auto obj : child)
        delete obj;
}

FSObject *Directory::clone() {
    return new Directory(*this);
}

bool Directory::isDirectory() const {
    return true;
}

bool Directory::isFile() const {
    return false;
}

void Directory::save(std::ofstream &dest) {
    FSObject::save(dest);
    size_t nchild = child.size();
    dest.write((char*)&nchild, sizeof(nchild));
    for(FSObject *object : child)
        object->save(dest);
}

FSObject *Directory::get(const std::string& name) {
    if(name == "..")
        return parent;
    if(name == ".")
        return this;
    for(auto obj : child) {
        if(obj->getName() == name)
            return obj;
    }
    return NULL;
}

Directory *Directory::getDirectory(const std::string& name) {
    FSObject *object = get(name);
    return object && object->isDirectory() ? (Directory*)object : NULL;
}

File *Directory::getFile(const std::string& name) {
    FSObject *object = get(name);
    return object && object->isFile() ? (File*)object : NULL;
}

void Directory::erase(FSObject *object) {
    auto pos = child.find(object);
    if(pos == child.end())
        return;
    child.erase(pos);
}

void Directory::add(FSObject *object) {
    if(object->isDirectory())
        ((Directory*)object)->parent = this;
    child.insert(object);
}

void Directory::show() {
    for(FSObject *obj : child) {
        if(obj->getName()[0] != '.')
            obj->display();
    }
}

bool Directory::has(Directory *other) {
    for(Directory *dir = this; dir; dir = dir->parent) {
        if(dir == other)
            return true;
    }
    return false;
}

