#include "FSObject.h"

FSObject::FSObject(const std::string& name, uint8_t attributes) : name{name}, attributes{attributes} {}

const std::string& FSObject::getName() const {
    return name;
}

bool FSObject::isDirectory() const {
    return (attributes & DIRECTORY) > 0;
}

bool FSObject::isFile() const {
    return !isDirectory();
}

bool FSObject::isHidden() const {
    return (attributes & HIDDEN) > 0;
}

bool FSObject::canRead() const {
    return (attributes & READ) > 0;
}

bool FSObject::canWrite() const {
    return (attributes & WRITE) > 0;
}

bool FSObject::canExecute() const {
    return (attributes & EXECUTE) > 0;
}

void FSObject::chmod(uint8_t mode) {
    attributes = (attributes & 0b10000000) | (mode & 0b00001111);
}

void FSObject::rename(const std::string& name) {
    this->name = name;
}
