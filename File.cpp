#include "File.h"
#include "FSObject.h"

File::File(const std::string& name, const std::string& content) : FSObject(name, FSObject::FILE | FSObject::READ | FSObject::WRITE | FSObject::EXECUTE), content{content} {}
File::File(const std::string& name) : File(name, "") {}

FSObject *File::clone() {
    return new File(*this);
}

std::string File::read() {
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
