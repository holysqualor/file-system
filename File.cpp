#include "File.h"
#include "FSObject.h"

File::File(const std::string& name) : FSObject(name, FSObject::FILE) {}

std::string File::read() {
    return content;
}

void File::write(const std::string& content) {
    this->content = content;
}
