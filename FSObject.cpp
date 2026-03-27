#include "FSObject.h"
#include "File.h"
#include "Directory.h"

FSObject::FSObject(const std::string& name, uint8_t mode) : name{name}, mode{mode} {}

void FSObject::readstr(std::ifstream &src, std::string &str) {
    size_t length = 0;
    src.read((char*)&length, sizeof(length));
    if(length) {
        str.resize(length);
        src.read(&str[0], length);
    }
}

void FSObject::writestr(std::ofstream &dest, std::string &str) {
    size_t length = str.length();
    dest.write((char*)&length, sizeof(length));
    if(length)
        dest.write(&str[0], length);
}

FSObject *FSObject::load(std::ifstream &src) {
    std::string name;
    readstr(src, name);
    uint8_t mode = 0;
    src.read((char*)&mode, sizeof(mode));
    if((mode & 0b1000) == 0) {
        std::string content;
        readstr(src, content);
        return new File(name, mode, content);
    }
    size_t nchild = 0;
    src.read((char*)&nchild, sizeof(nchild));
    Directory *directory = new Directory(name, mode);
    for(size_t i = 0; i < nchild; i++)
        directory->add(load(src));
    return directory;
}

void FSObject::save(std::ofstream &dest) {
    writestr(dest, name);
    mode = isFile() ? (mode & 0b0111) : (mode | 0b1000);
    dest.write((char*)&mode, sizeof(mode));
}

const std::string& FSObject::getName() const {
    return name;
}

bool FSObject::canRead() const {
    return (mode & READ) > 0;
}

bool FSObject::canWrite() const {
    return (mode & WRITE) > 0;
}

bool FSObject::canExecute() const {
    return (mode & EXECUTE) > 0;
}

void FSObject::chmod(uint8_t mode) {
    this->mode = mode;
}

void FSObject::rename(const std::string& name) {
    this->name = name;
}

void FSObject::display() const {
    std::cout << (isDirectory() ? 'd' : '-') << (isFile() ? 'f' : '-') << (canRead() ? 'r' : '-')
    << (canWrite() ? 'w' : '-') << (canExecute() ? 'x' : '-') << '\t' << name << std::endl;
}
