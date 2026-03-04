#include "FSObject.h"

FSObject::FSObject(const std::string& name, uint8_t attributes) : name{name}, attributes{attributes} {}

FSObject::FSObject(const std::string& name, bool type) : FSObject(name, (uint8_t)(type ? 0b10000111 : 0b0111)) {}

const std::string& FSObject::getName() const {
    return name;
}

bool FSObject::isDir() {
    return (attributes & 0b10000000) > 0;
}

bool FSObject::isHidden() {
    return (attributes & 0b00001000) > 0;
}

bool FSObject::canRead() {
    return (attributes & 0b00000100) > 0;
}

bool FSObject::canWrite() {
    return (attributes & 0b00000010) > 0;
}

bool FSObject::canExecute() {
    return (attributes & 0b00000001) > 0;
}

void FSObject::chmod(bool r, bool w, bool x) {
    attributes = (attributes & 0b10001000) | (r ? 4 : 0) | (w ? 2 : 0) | (w ? 1 : 0);
}

void FSObject::rename(const std::string& name) {
    this->name = name;
}
