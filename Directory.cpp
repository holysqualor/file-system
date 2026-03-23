#include "Directory.h"

Directory::Directory(const std::string& name, Directory *parent) : FSObject(name, FSObject::READ | FSObject::WRITE | FSObject::EXECUTE), parent{parent} {}

Directory::Directory() : Directory("", NULL) {}

Directory::Directory(const Directory& other) : FSObject(other), parent{NULL} {
    for(FSObject *obj : other.child) {
        FSObject *cp = obj->clone();
        if(cp->isDirectory())
            ((Directory*)cp)->parent = this;
        child.push_back(cp);
    }
}

Directory::~Directory() {
    for(FSObject *obj : child)
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

std::string Directory::getType() const {
    return "Directory";
}

FSObject *Directory::find(const std::string& name) {
    if(name == "..")
        return parent;
    if(name == ".")
        return this;
    for(FSObject *obj : child) {
        if(obj->getName() == name)
            return obj;
    }
    return NULL;
}

File *Directory::getFile(const std::string& name) {
    FSObject *obj = find(name);
    return obj && obj->isFile() ? (File*)obj : NULL;
}

Directory *Directory::getDirectory(const std::string& name) {
    FSObject *obj = find(name);
    return obj && obj->isDirectory() ? (Directory*)obj : NULL;
}

void Directory::addFile(const std::string& name) {
    child.push_back(new File(name));
}

void Directory::addFile(const std::string& name, const std::string& content) {
    child.push_back(new File(name, content));
}

void Directory::addDirectory(const std::string& name) {
    child.push_back(new Directory(name, this));
}

void Directory::rm(const std::string& name) {
    if(name == "..")
        return;
    for(auto it = child.begin(); it != child.end(); it++) {
        FSObject *obj = *it;
        if(obj->getName() == name) {
            child.erase(it);
            delete obj;
            break;
        }
    }
}

FSObject *Directory::pop(const std::string& name) {
    for(auto it = child.begin(); it != child.end(); it++) {
        FSObject *obj = *it;
        if(obj->getName() == name) {
            child.erase(it);
            return obj;
        }
    }
    return NULL;
}

void Directory::push(FSObject *obj) {
    if(obj->isDirectory())
        ((Directory*)obj)->parent = this;
    child.push_back(obj);
}

void Directory::ls() {
    for(FSObject *obj : child)
        obj->display();
}

void Directory::pwd() {
    if(parent)
        parent->pwd();
    std::cout << getName() << "/";
}

bool Directory::isindp(Directory *other) {
    for(Directory *dir = this; dir; dir = dir->parent) {
        if(dir == other)
            return false;
    }
    return true;
}

