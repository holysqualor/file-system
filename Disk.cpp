#include "Disk.h"

Disk::Disk() : root{new Directory()}, current{root} {}

Disk::~Disk() {
    delete root;
}

void Disk::touch(const std::string& name) {
    current->addFile(name);
}

void Disk::mkdir(const std::string& name) {
    current->addDirectory(name);
}

void Disk::cd(const std::string& name) {
    if(name.empty())
        current = root;
    Directory *dir = current->getDirectory(name);
    if(dir)
        current = dir;
}

void Disk::rename(const std::string& oldName, const std::string& newName) {
    current->rename(oldName, newName);
}

void Disk::echo(const std::string& name, const std::string& content) {
    File *file = current->getFile(name);
    if(file)
        file->write(content);
}
void Disk::cat(const std::string& name) {
    File *file = current->getFile(name);
    if(file)
        std::cout << file->read() << std::endl;
}

void Disk::ls() const {
    current->ls();
}
