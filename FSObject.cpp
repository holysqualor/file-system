#include "FSObject.h"

FSObject::FSObject(const std::string& name, uint8_t mode) : name{name}, mode{mode} {}

const std::string& FSObject::getName() const {
    return name;
}

std::string FSObject::getType() const {
    return "Undefined";
}

bool FSObject::isHidden() const {
    return (mode & HIDDEN) > 0;
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
