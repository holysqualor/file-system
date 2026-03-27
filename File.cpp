#include "File.h"
#include "FSObject.h"

File::File(const std::string& name, uint8_t mode, const std::string& content) : FSObject(name, mode), content{content} {}
File::File(const std::string& name, const std::string& content) : File(name, FSObject::READ | FSObject::WRITE | FSObject::EXECUTE, content) {}
File::File(const std::string& name) : File(name, "") {}

FSObject *File::clone() {
    return new File(*this);
}

bool File::isDirectory() const {
    return false;
}

bool File::isFile() const {
    return true;
}

void File::save(std::ofstream &dest) {
    FSObject::save(dest);
    writestr(dest, content);
}

const std::string &File::read() const {
    return content;
}

void File::write(const std::string& content) {
    this->content = content;
}
