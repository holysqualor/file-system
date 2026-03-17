#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include "FSObject.h"

#include <iostream>
#include <string>
#include <cstdint>

class File : public FSObject {
private:
    std::string content;

public:
    File(const std::string& name);
    ~File() = default;

    std::string read();
    void write(const std::string& name);

    /*friend std::ostream& operator<<(std::ostream& os, const File& file) {
        if(file.canRead())
            os << file.content;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, File& file) {
        if(file.canWrite())
            is >> file.content;
        return is;
    }*/
};

#endif // FILE_H_INCLUDED
