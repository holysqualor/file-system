#include "File.h"
#include "FSObject.h"

File::File(const std::string& name, const std::string& content) : FSObject(name, FSObject::READ | FSObject::WRITE | FSObject::EXECUTE), content{content} {}
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

std::string File::getType() const {
    return "File";
}

const std::string &File::read() const {
    return content;
}

void File::write(const std::string& content) {
    this->content = content;
}

std::ostream& operator<<(std::ostream& os, const File& file) {
    os << file.content;
    return os;
}

std::istream& operator>>(std::istream& is, File& file) {
    is >> file.content;
    return is;
}
